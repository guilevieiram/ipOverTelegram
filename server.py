from flask import Flask, request
import requests

app = Flask(__name__)

COUNTER = 0
@app.route("/")
def home():
    """Home method used to define the counter endpoint."""
    global COUNTER
    COUNTER += 1
    return f"Hello counter {COUNTER}\n"

@app.route("/getUrl", methods=["GET"])
def get_url():
    """Endpoint to perform a get request to the passed url."""
    url = request.args.get("url")
    res = requests.get(url, timeout=10)
    return res.text + "\n"

if __name__=="__main__":
    app.run(host="10.0.0.2", port="8000")
