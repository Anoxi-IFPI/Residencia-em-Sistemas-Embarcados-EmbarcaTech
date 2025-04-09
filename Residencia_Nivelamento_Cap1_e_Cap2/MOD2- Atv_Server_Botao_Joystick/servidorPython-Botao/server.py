from flask import Flask, request, render_template, jsonify
from datetime import datetime
import os

app = Flask(__name__)
app.config['TEMPLATES_AUTO_RELOAD'] = True

# Lista para armazenar mensagens
message_history = []

@app.route("/")
def home():
    return render_template('index.html')

@app.route("/mensagem", methods=["GET"])
def receber_mensagem():
    mensagem = request.args.get("msg", "Sem mensagem")

    timestamp = datetime.now().strftime("%H:%M:%S")
    formatted_msg = f"[{timestamp}] {mensagem}"
    
    message_history.append(formatted_msg)
    print(formatted_msg)  # Log no console
    
    # Mantém apenas as últimas 50 mensagens
    if len(message_history) > 10:
        message_history.pop(0)
    
    return "Mensagem recebida", 200

@app.route("/get_messages")
def get_messages():
    return jsonify(message_history)

if __name__ == "__main__":
    # Cria a pasta templates se não existir
    if not os.path.exists('templates'):
        os.makedirs('templates')
    
    app.run(host="0.0.0.0", port=5000, debug=True)