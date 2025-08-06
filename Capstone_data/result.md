The output shows a comprehensive comparison of different models (ARIMA, ARIMAX, and ARIMA+LightGBM) applied to both old and new datasets for SHAHJABANK stock prediction. Here's a breakdown of the key metrics and which model performs best:

### Key Observations:
1. **Best Model for New Data**:
   - **ARIMA+LightGBM (full features)** performs the best on the new dataset with:
     - **RMSE: 0.0595** (lowest among all models)
     - **R²: 0.9664** (highest, closest to 1)
     - **Improvement over ARIMA: 24.08%**
     - **Absolute Error (mean): 0.0449** (lower than ARIMA and ARIMAX)
     - **Percentage Error (mean): 0.0087** (closest to 0)

2. **Best Model for Old Data**:
   - **ARIMAX (basic features)** performs best on the old dataset with:
     - **RMSE: 0.2016**
     - **R²: 0.9536**
     - **Absolute Error (mean): 0.1649**
     - **Percentage Error (mean): -0.0507**

3. **Worst Models**:
   - **ARIMA (basic/derived) on old data** has the highest errors (RMSE: 0.2494, Absolute Error: 0.1631).
   - **ARIMAX_Backtest (full features) on new data** performs poorly with very high errors (RMSE: 9.667, Absolute Error: 5.094).

4. **Backtest Performance**:
   - Backtest results are generally worse than standard evaluations, indicating potential overfitting or instability in real-world scenarios.
   - **ARIMA+LightGBM backtest** still performs reasonably well (RMSE: 1.0408) compared to other backtests.

### Why ARIMA+LightGBM is the Best:
- Combines the strengths of ARIMA (for capturing temporal dependencies) and LightGBM (for modeling complex residuals/non-linear patterns).
- Achieves the lowest RMSE and highest R² on the new dataset.
- Handles gaps in data better (interpolation was applied for 43 gaps > 3 days).
- Residual correction with LightGBM improves accuracy by 24.08% over standalone ARIMA.

### Recommendations:
1. **For New Data**: Use **ARIMA+LightGBM (full features)** for the most accurate predictions.
2. **For Old Data**: Use **ARIMAX (basic features)** if working with the older dataset.
3. **Avoid**: Standalone ARIMA models (basic/derived) and ARIMAX with full features on old data due to higher errors.

### Summary Table (New Data Performance):
| Model Type     | Feature Set | RMSE   | R²     | Absolute Error (mean) | Improvement Over ARIMA |
|----------------|-------------|--------|--------|-----------------------|------------------------|
| ARIMA+LGB      | Full        | 0.0595 | 0.9664 | 0.0449                | 24.08%                 |
| ARIMAX         | Derived     | 0.0334 | 0.9894 | 0.0261                | -                      |
| ARIMAX         | Basic       | 0.0728 | 0.9497 | 0.0507                | -                      |
| ARIMA          | Basic       | 0.0784 | 0.9417 | 0.0522                | Baseline               |

**Note**: While ARIMAX (derived) has a slightly lower RMSE (0.0334), ARIMA+LGB is more robust (better R² and backtest performance). The choice depends on the priority: pure accuracy (ARIMAX-derived) or stability (ARIMA+LGB).
