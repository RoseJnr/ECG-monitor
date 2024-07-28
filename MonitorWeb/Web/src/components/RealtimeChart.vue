<template>
  <div>
    <Line :data="chartData" :options="options" ref="lineChart"></Line>
  </div>
</template>

<script>
import { Line } from 'vue-chartjs';
import { defineComponent, ref, onMounted} from 'vue';
import { Chart as ChartJS, Title, Tooltip, Legend, LineElement, CategoryScale, LinearScale, PointElement } from 'chart.js';

ChartJS.register(Title, Tooltip, Legend, LineElement, CategoryScale, LinearScale, PointElement);

export default defineComponent({
  name: 'RealtimeChart',
  components: {
    Line
  },
  props: {
    chartData: {
      type: Object,
      required: true
    }
  },
  setup() {
    const lineChart = ref(null);
    const options = {
      responsive: true,
      maintainAspectRatio: false
    };

    onMounted(() => {
      // watch(
      //   () => props.chartData,
      //   (newData) => {
      //     console.log("New data: ", newData);
      //     if (lineChart.value && lineChart.value.chartInstance) {
      //       console.log('Updating chart instance data');
      //       lineChart.value.chartInstance.data = JSON.parse(JSON.stringify(newData));
      //       lineChart.value.chartInstance.update();
      //     }
      //   },
      //   { deep: true }
      // );
    });

    return {
      options,
      lineChart
    };
  }
});
</script>

<style scoped>
</style>
