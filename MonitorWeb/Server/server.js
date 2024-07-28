// const express = require('express');
// const http = require('http');
// const socketIo = require('socket.io');
// const bodyParser = require('body-parser');
// const { InfluxDB, Point } = require('@influxdata/influxdb-client');

// // Substitua pelos seus valores
// const token = '96nfBtVrv_BUBIoLY2hgwBxjTGO8L65KbnGZg9yejZ7b9FMm_iwjANl_uEO_bv8bXFE_zpZBGZG7PzCKiO-a9g==';
// const org = 'Unifesp';
// const bucket = 'ecg_data';
// const client = new InfluxDB({ url: 'http://localhost:8086', token: token });
// const writeApi = client.getWriteApi(org, bucket);
// writeApi.useDefaultTags({ host: 'localhost' });

// const app = express();
// const server = http.createServer(app);
// // const { Server } = require("socket.io");
// // const io = new Server(server);
// // const io = socketIo(server);
// const io = require('socket.io')(http, {
//   cors: {
//     origins: ['http://localhost:8080']
//   }
// });

// app.use(bodyParser.json());

// app.post('/data', (req, res) => {
//   const ecgData = req.body;
//   console.log('Dados recebidos do ESP32:', ecgData);  // Log dos dados recebidos

//   io.emit('new_ecg_data', ecgData);
//   res.status(200).send('Data received');

//   // Salvar os dados no InfluxDB
//   // const point = new Point('ecg')
//   //   .intField('value', ecgData.value)
//   //   .timestamp(new Date(ecgData.timestamp));
//   // writeApi.writePoint(point);

//   // writeApi.flush().then(() => {
//   //   console.log('Dados salvos no InfluxDB');
//   // }).catch(err => {
//   //   console.error(`Erro ao salvar dados no InfluxDB: ${err.stack}`)
//   // });
// });

// io.on('connection', (socket) => {
//   console.log('Novo cliente conectado');  // Log quando um cliente se conecta
//   socket.on('disconnect', () => {
//     console.log('Cliente desconectado');  // Log quando um cliente se desconecta
//   });
// });

// server.listen(3000, () => {
//   console.log('Servidor está rodando na porta 3000');
// });





// New server
let x = 0;
const app = require('express')();
const http = require('http').createServer(app);
const io = require('socket.io')(http, {
  cors: {
    origin: 'http://localhost:8080'
  }
});

const bodyParser = require('body-parser');

// Middleware para parsing de JSON
app.use(bodyParser.json());

app.get('/', (req, res) => {
  res.send('<h1>Hey Socket.io</h1>');
});

// Endpoint para receber dados da ESP32
app.post('/data', (req, res) => {
  const data = req.body;
  console.log('Received data from ESP32:', data);

  // Emitir os dados para os clientes conectados
  if (Boolean(x))
  {
    x = 0;
    io.emit('esp32-data', data);
    console.log('Data emitted to clients');
  }
  else
  {
    x = 1;
  }
  res.status(200).send('Data received');
});

io.on('connection', (socket) => {
  console.log('a user connected');
  socket.on('disconnect', () => {
    console.log('user disconnected');
  });
});

http.listen(3000, () => {
  console.log('listening on *:3000');
});
