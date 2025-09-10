Here’s what your script is doing end-to-end, and the exact math behind each step. The key idea: the model predicts daily returns (not prices) from technical features, then converts predicted returns into prices by compounding.

High-level flow
- Data ingestion and cleaning: Load OHLCV, remove outliers, keep Bangladesh business days (Sun–Thu).
- Feature engineering: Build 3 features: 5-day moving average (MA5), RSI(14), and volume percent change. Target is daily return.
- Scaling: Scale features and target into [-1, 1] using MinMaxScaler.
- Sequence creation: Build sliding windows of the last 60 days of features to predict the next day’s return.
- Model: 3-layer Bidirectional LSTM + Dense, trained to minimize MSE on scaled returns.
- Walk-forward monthly training: Append new month’s data, incrementally update model.
- Forecasting: Predict future returns for the next PREDICTION_DAYS, then convert those returns into prices by compounding from the last known price.
- Evaluation: If actuals are available, compute MAE and RMSE and plot predicted vs actual.

What the model actually predicts
- The model predicts next-day return r_t from a 60-day window of features up to day t−1.
- Price predictions are derived afterward:
  - P̂_{t+1} = P_t (1 + r̂_{t+1})
  - P̂_{t+h} = P_t ∏_{j=1}^{h} (1 + r̂_{t+j})

Mathematical details and all equations

1) Target and feature engineering
- Daily return (target):
  r_t = (close_t - close_{t-1}) / close_{t-1} = close_t / close_{t-1} - 1

- 5-day moving average:
  MA5_t = (1/5) ∑_{k=0}^{4} close_{t-k}

- RSI(14) (using simple rolling means):
  Δ_t = close_t - close_{t-1}
  gain_t = max(Δ_t, 0)
  loss_t = max(-Δ_t, 0)

  ḡ_t = (1/14) ∑_{k=0}^{13} gain_{t-k}
  l̄_t = (1/14) ∑_{k=0}^{13} loss_{t-k} + ε  with ε = 10^{-10} (to avoid divide-by-zero)

  RS_t = ḡ_t / l̄_t
  RSI_t = 100 - 100 / (1 + RS_t)

- Volume percent change:
  vc_t = (volume_t - volume_{t-1}) / volume_{t-1}

- IQR outlier filtering (on close):
  Q1 = quantile_{0.25}(close), Q3 = quantile_{0.75}(close), IQR = Q3 - Q1
  Keep rows with close ∈ [Q1 - 1.5·IQR, Q3 + 1.5·IQR]

2) Business day filter (Bangladesh)
- Keep dates whose weekday ∉ {4, 5} (Friday=4, Saturday=5).

3) Scaling to [-1, 1] (features and target)
Using MinMaxScaler with feature_range [a, b] = [-1, 1].

For any value x of a given variable with training min x_min and max x_max:

- Forward transform:
  x_scaled = a + (x - x_min) · (b - a) / (x_max - x_min)
  Here: a = -1, b = 1 ⇒ x_scaled = 2 · (x - x_min) / (x_max - x_min) - 1

- Inverse transform (used to recover returns from model output):
  x = x_min + (x_scaled - a) · (x_max - x_min) / (b - a)
  With [-1, 1]: x = x_min + (x_scaled + 1) · (x_max - x_min) / 2

4) Sequence construction for LSTM
With lookback L = 60, features f_t ∈ R^3, and target r_t:

- For indices i = L, L+1, …, T−1:
  X_i = [f_{i-L}, f_{i-L+1}, …, f_{i-1}]  (shape 60×3)
  y_i = r_i

This aligns “next-day” return r_i with features up to the previous day.

5) LSTM model (Bidirectional)
Each LSTM cell (per direction) uses the standard equations:
- Gates and candidate:
  i_t = σ(W_i x_t + U_i h_{t-1} + b_i)
  f_t = σ(W_f x_t + U_f h_{t-1} + b_f)
  o_t = σ(W_o x_t + U_o h_{t-1} + b_o)
  g_t = tanh(W_g x_t + U_g h_{t-1} + b_g)

- Cell and hidden states:
  c_t = f_t ⊙ c_{t-1} + i_t ⊙ g_t
  h_t = o_t ⊙ tanh(c_t)

Bidirectional LSTM concatenates forward and backward hidden states:
  ĥ_t = concat(h_t^{→}, h_t^{←})

Your architecture:
- 3 stacked BiLSTM layers (each with “return sequences” except the last), dropout 0.2 after each.
- Dense layer with ReLU: a = ReLU(W_d ĥ + b_d)
- Output: ŷ_scaled = W_o a + b_o (linear output in the scaled space)

6) Loss and metrics
- Training loss: Mean Squared Error (MSE) on scaled returns
  MSE = (1/N) ∑_{i=1}^{N} (ŷ_i - y_i)^2

- Tracked metric: Mean Absolute Error (MAE)
  MAE = (1/N) ∑_{i=1}^{N} |ŷ_i - y_i|

7) Optimization (Adam)
Let g_t = ∇_θ L_t be the gradient at step t:

- First and second moment estimates:
  m_t = β_1 m_{t-1} + (1 - β_1) g_t
  v_t = β_2 v_{t-1} + (1 - β_2) g_t^2

- Bias correction:
  m̂_t = m_t / (1 - β_1^t)
  v̂_t = v_t / (1 - β_2^t)

- Parameter update:
  θ_{t+1} = θ_t - α · m̂_t / (√(v̂_t) + ε)

With learning rate α (initially 0.001), β_1 ≈ 0.9, β_2 ≈ 0.999, ε ≈ 10^{-8}.

- ReduceLROnPlateau reduces α by factor 0.5 if loss plateaus (down to min 1e-6).
- EarlyStopping stops if training loss stops improving.

8) Architecture adaptation (defined, but not effectively used in main loop due to missing error updates)
- Compute recent error trend (slope m via simple linear regression on last K errors e_t):
  m = [∑_{t=1}^{K} (t - t̄)(e_t - ē)] / [∑_{t=1}^{K} (t - t̄)^2]

- Decisions:
  If m > 0 and avg_error > 75th percentile of past errors → increase LSTM units by +10 (cap at 200), increase LR slightly.
  If m < 0 and avg_error < 25th percentile → decrease units by −5 (floor at 30), decrease LR slightly.

9) Predictive error correction (defined but not applied to predictions)
- Using last 5 errors e_{t-4..t}, define trend m_5 and mean ē_5:
  correction = 0.3 · (ē_5 + 2 m_5)

10) Multi-step forecasting of returns
- Let last_sequence be the last 60 days of scaled features.
- For step k = 1..H:
  1) ŷ_{t+k}^{scaled} = f_θ(input_seq_k)  where input_seq_k is the last 60 steps (zeros appended for unknown future features).
  2) r̂_{t+k} = inverse_scale(ŷ_{t+k}^{scaled})  (using target scaler)
  3) Append a zero feature vector for the next step (since future features are unknown).

Note: future features are set to zeros, which is a simplifying assumption and can degrade multi-step accuracy.

11) Convert predicted returns to prices
- Starting from last known price P_t:
  P̂_{t+1} = P_t (1 + r̂_{t+1})
  P̂_{t+2} = P̂_{t+1} (1 + r̂_{t+2}) = P_t ∏_{j=1}^{2} (1 + r̂_{t+j})
  …
  P̂_{t+h} = P_t ∏_{j=1}^{h} (1 + r̂_{t+j})

12) Evaluation on overlap (if actuals are available)
- MAE and RMSE between predicted prices and actual closes:
  MAE = (1/N) ∑ |P̂_t - P_t|
  RMSE = √[(1/N) ∑ (P̂_t - P_t)^2]

Key assumptions and caveats
- The model learns to predict next-day return r_t from features up to t−1 (proper one-step-ahead setup).
- Future features during forecasting are unknown and replaced with zeros. This is a strong simplification; a more robust approach would simulate or forecast the features too, or use a univariate model on returns.
- Scaling is fit only on the initial training window and reused later; distribution shifts can affect inverse scaling accuracy for returns.

If you want, I can:
- Rewrite the sequence/target alignment diagrammatically so you can see exact indices.
- Replace the “zeros for future features” with a better recursive feature simulation.
- Add direct price modeling (e.g., predict log-returns or price deltas) and compare.
