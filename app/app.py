# app.py

from flask import Flask, render_template, jsonify
import json

app = Flask(__name__, static_url_path='/static', static_folder='static')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/get_json_data')
def get_json_data():
    # Cargar y procesar el JSON
    with open('../cli/data_block.json') as json_file:
        json_data = json.load(json_file)
    
    blocks_data = []
    for block_key in json_data[0]:
        block_data = json_data[0][block_key][0]
        
        # Obtener información de transacciones
        transactions = block_data.get('transactions', [])
        formatted_transactions = []
        for transaction in transactions:
            inputs = transaction.get('inputs', [])
            outputs = transaction.get('outputs', [])
            formatted_transaction = {
                'transaction_id': transaction.get('transaction_id', ''),
                'inputs': [{'block_hash': input.get('block_hash', ''),
                            'transaction_id': input.get('transaction_id', ''),
                            'transaction_output_hash': input.get('transaction_output_hash', '')}
                        for input in inputs],
                'outputs': [{'amount': output.get('amount', ''),
                            'public_address': output.get('public_address', ''),
                            'output_hash': output.get('output_hash', ''),
                            'status': output.get('status', '')}
                            for output in outputs]
            }
            formatted_transactions.append(formatted_transaction)

        blocks_data.append({
            'index': block_data['index'],
            'difficulty': block_data['difficulty'],
            'timestamp': block_data['timestamp'],
            'nonce': block_data['nonce'],
            'previous_block_hash': block_data['previous_block_hash'],
            'block_hash': block_data['block_hash'],
            'block_size': block_data['block_size'],
            'num_transactions': block_data['num_transactions'],
            'transactions': formatted_transactions
        })

    return jsonify(blocks_data)

@app.route('/get_blockchain_data')
def get_blockchain_data():
    # Cargar y procesar el JSON de blockchain
    with open('../cli/data_blockchain.json') as json_file:
        blockchain_data = json.load(json_file)

    return jsonify(blockchain_data)

@app.route('/get_utxo_data')
def get_utxo_data():
    try:
        # Lee los datos del archivo data_utxo.json
        with open('../cli/data_utxo.json', 'r') as file:
            utxo_data = json.load(file)
        # Retorna los datos en formato JSON
        return jsonify(utxo_data)
    except Exception as e:
        # Retorna un mensaje de error si ocurre algún problema
        return jsonify({'error': str(e)})

if __name__ == '__main__':
    app.run(debug=True)
