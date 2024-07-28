<template>
  <div id="app">
    <RealtimeChart :chartData="datacollection"></RealtimeChart>
  </div>
</template>

<script>
import SocketioService from './services/socketio.service.js';
import RealtimeChart from './components/RealtimeChart.vue';

export default {
  name: 'App',
  components: {
    RealtimeChart
  },
  data() {
    return {
      datacollection: {
        labels: [],
        datasets: [
          {
            label: 'ESP32 Data',
            backgroundColor: '#f87979',
            borderColor: '#f87979',
            data: []
          }
        ]
      }
    };
  },
  created() {
    SocketioService.setupSocketConnection();

    SocketioService.socket.on('connect', () => {
      console.log('Connected to server');
    });

    SocketioService.socket.on('esp32-data', (data) => {
      console.log('Data received from ESP32:', data);
      this.updateChartData(data);
    });

    SocketioService.socket.on('disconnect', () => {
      console.log('Disconnected from server');
    });
  },
  methods: {
    updateChartData(data) {
      if (this.datacollection) {
        this.datacollection.labels.push(new Date(data.timestamp).toLocaleTimeString());
        const oldData = [...this.datacollection.datasets[0].data]
        oldData.push(data.value)
        this.datacollection.datasets[0].data = [...oldData]
        // Keep only the last 20 points in the chart to keep it readable
        if (this.datacollection.labels.length > 200) {
          this.datacollection.labels.shift();
          this.datacollection.datasets[0].data.shift();
        }

        this.datacollection = {...this.datacollection}
      }
    }
  }
}
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
