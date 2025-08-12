### **Production-Ready Stock Price Prediction System for SHAHJABANK**

Here's a structured approach to deploy your ARIMA-based stock prediction system in production, incorporating best practices for reliability, scalability, and maintainability.

---

## **1. System Architecture**
### **A. Components**
1. **Data Pipeline**  
   - Fetch OHLCV data daily (automated).  
   - Clean/validate data (handle missing values, outliers).  
   - Store in a database (e.g., PostgreSQL, TimescaleDB).  

2. **Model Training & Serving**  
   - Retrain ARIMA periodically (e.g., weekly).  
   - Serve predictions via API (FastAPI/Flask).  

3. **Monitoring & Alerting**  
   - Track prediction accuracy (RMSE/MAPE).  
   - Alert if drift exceeds thresholds.  

4. **Frontend (Optional)**  
   - Display forecasts (e.g., Dash/Streamlit).  

---

## **2. Production Implementation Steps**

### **Step 1: Data Pipeline Automation**
#### **Option A: Scheduled Script (Cron/Airflow)**
```python
# fetch_data.py (run daily at market close)
import pandas as pd
from datetime import datetime
from bdshare import get_hist_data

def fetch_and_save_data():
    company = "SHAHJABANK"
    end_date = datetime.now().strftime('%Y-%m-%d')
    start_date = (datetime.now() - timedelta(days=7)).strftime('%Y-%m-%d')  # Weekly refresh
    
    df = get_hist_data(start_date, end_date, company)
    if df is not None:
        df.to_csv(f"data/latest_{company}.csv", index=False)
        append_to_database(df)  # Pseudocode for DB storage

# Schedule with cron:
# 0 18 * * * /usr/bin/python3 /path/to/fetch_data.py
```

#### **Option B: Airflow DAG (Advanced)**
```python
# airflow_dag.py
from airflow import DAG
from airflow.operators.python import PythonOperator
from datetime import datetime, timedelta

def fetch_data():
    # Same as above

default_args = {
    'owner': 'airflow',
    'retries': 3,
    'retry_delay': timedelta(minutes=5),
}

dag = DAG(
    'stock_data_pipeline',
    default_args=default_args,
    schedule_interval='0 18 * * *',  # 6 PM daily
    start_date=datetime(2023, 1, 1),
)

task = PythonOperator(
    task_id='fetch_shahjabank_data',
    python_callable=fetch_data,
    dag=dag,
)
```

---

### **Step 2: ARIMA Model Training**
#### **Auto-ARIMA with Retraining**
```python
# train_model.py
import pandas as pd
from pmdarima import auto_arima
import joblib

def train_arima():
    df = pd.read_csv("data/latest_SHAHJABANK.csv")
    df['date'] = pd.to_datetime(df['date'])
    df.set_index('date', inplace=True)
    
    model = auto_arima(
        df['close'],
        seasonal=False,
        trace=True,
        error_action='ignore',
        suppress_warnings=True,
    )
    joblib.dump(model, "models/arima_model.joblib")

# Retrain weekly via cron/Airflow
```

---

### **Step 3: Prediction API (FastAPI)**
```python
# app.py
from fastapi import FastAPI
import joblib
import pandas as pd
from datetime import datetime, timedelta

app = FastAPI()
model = joblib.load("models/arima_model.joblib")

@app.get("/predict")
def predict(days: int = 7):
    # Load latest data
    df = pd.read_csv("data/latest_SHAHJABANK.csv")
    last_date = pd.to_datetime(df['date'].iloc[-1])
    
    # Forecast
    forecast = model.predict(n_periods=days)
    dates = [last_date + timedelta(days=i) for i in range(1, days+1)]
    
    return {
        "forecast_dates": [d.strftime('%Y-%m-%d') for d in dates],
        "forecast_prices": forecast.tolist(),
    }

# Run with: uvicorn app:app --reload
```

---

### **Step 4: Deployment**
#### **A. Local/Cloud Setup**
1. **Database**:  
   - Use PostgreSQL with TimescaleDB for time-series data.  
   - Table schema:  
     ```sql
     CREATE TABLE stock_data (
         date DATE PRIMARY KEY,
         symbol VARCHAR(20),
         open FLOAT,
         high FLOAT,
         low FLOAT,
         close FLOAT,
         volume INT
     );
     ```

2. **API Deployment**:  
   - Deploy FastAPI with Gunicorn + Nginx (or use serverless like AWS Lambda).  
   - Example Gunicorn command:  
     ```bash
     gunicorn -w 4 -k uvicorn.workers.UvicornWorker app:app
     ```

#### **B. Containerization (Docker)**
```dockerfile
# Dockerfile
FROM python:3.9-slim

WORKDIR /app
COPY requirements.txt .
RUN pip install -r requirements.txt

COPY . .

CMD ["gunicorn", "-w", "4", "-k", "uvicorn.workers.UvicornWorker", "app:app"]
```

---

### **Step 5: Monitoring & Maintenance**
1. **Logging**:  
   - Use `logging` module + ELK Stack (Elasticsearch, Logstash, Kibana).  
2. **Alerts**:  
   - Set up Slack alerts for prediction drift (e.g., if RMSE > 5%).  
3. **Retraining**:  
   - Weekly cron job:  
     ```bash
     0 3 * * MON /usr/bin/python3 /path/to/train_model.py
     ```

---

## **3. Production Recommendations**
1. **Start Simple**:  
   - Begin with cron-scheduled scripts + FastAPI (no need for Airflow initially).  
2. **Scale Later**:  
   - Add Airflow if workflows become complex.  
   - Switch to Kubernetes if handling multiple stocks.  
3. **Backtesting**:  
   - Validate models with historical data before full deployment.  
4. **Legal Compliance**:  
   - Disclaim that predictions are not financial advice.  

---

## **4. Example Workflow**
1. **Daily**:  
   - Fetch new data → Store in DB → Update CSV.  
2. **Weekly**:  
   - Retrain ARIMA → Save model.  
3. **Real-Time**:  
   - API serves latest predictions.  

---

### **Final Notes**
- **ARIMA Alone is Not Enough**: For better accuracy, consider:  
  - **LSTM Hybrid**: Use ARIMA for baseline + LSTM for residuals.  
  - **ARIMAX**: Incorporate volume as an exogenous variable.  
- **Cost Optimization**: Use spot instances for training in AWS/GCP.  

Would you like help setting up any specific component (e.g., FastAPI, Airflow)?
