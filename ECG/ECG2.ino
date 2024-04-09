#include <Arduino.h>

// Definições de pinos
const int pinSensor = A0;

// Variáveis para filtragem (ajuste conforme necessário)
float lastFilteredValue = 0;
const float alpha = 0.1; // Coeficiente de filtragem simplificado

// Buffer para cálculo da média e desvio padrão
const int bufferSize = 128; // Tamanho do buffer de amostras para normalização
float sampleBuffer[bufferSize];
int sampleIndex = 0;
float sumSamples = 0;
float mean = 0;

// Buffer para o filtro mediano
float medianBuffer[3];

void setup() {
  Serial.begin(115200); // Inicia a comunicação serial
  for (int i = 0; i < bufferSize; i++) {
    sampleBuffer[i] = 0; // Inicializa o buffer de amostras
  }
}

float simplePassBandFilter(float input) {
  // Filtro IIR simples: y[n] = (1-alpha)*y[n-1] + alpha*x[n]
  lastFilteredValue = (1 - alpha) * lastFilteredValue + alpha * input;
  return lastFilteredValue;
}

float applyMedianFilter(float newValue) {
  medianBuffer[0] = medianBuffer[1];
  medianBuffer[1] = medianBuffer[2];
  medianBuffer[2] = newValue;

  // Copia o buffer para realizar a ordenação
  float sortedBuffer[3];
  memcpy(sortedBuffer, medianBuffer, 3 * sizeof(float));
  
  // Ordenação simples (Bubble Sort para exemplo)
  for(int i=0; i<2; i++) {
    for(int j=i+1; j<3; j++) {
      if(sortedBuffer[i] > sortedBuffer[j]) {
        float temp = sortedBuffer[i];
        sortedBuffer[i] = sortedBuffer[j];
        sortedBuffer[j] = temp;
      }
    }
  }

  // Retorna o valor mediano
  return sortedBuffer[1];
}

float calculateSD(float mean, float samples[], int numSamples) {
  float variance = 0;
  for (int i = 0; i < numSamples; i++) {
    variance += pow(samples[i] - mean, 2);
  }
  variance /= numSamples;
  return sqrt(variance);
}

void loop() {
  static unsigned long lastSampleTime = 0;
  static int sampleCount = 0;
  unsigned long currentSampleTime = millis();

  if (currentSampleTime - lastSampleTime >= 4) { // Amostragem a 250Hz
    lastSampleTime = currentSampleTime;
    
    int valorLido = analogRead(pinSensor);
    float filteredValue = simplePassBandFilter(valorLido);
    // float medianValue = applyMedianFilter(filteredValue);

    // Atualiza buffer, média e desvio padrão
    sumSamples -= sampleBuffer[sampleIndex];
    sampleBuffer[sampleIndex] = filteredValue;
    sumSamples += filteredValue;
    sampleIndex = (sampleIndex + 1) % bufferSize;
    if (sampleIndex % bufferSize == 0) { // Recalcula a média e desvio padrão após preencher o buffer
      mean = sumSamples / bufferSize;
    }

    // Downsampling a 125 Hz
    if (++sampleCount % 2 == 0) {
      float sd = calculateSD(mean, sampleBuffer, bufferSize); // Calcula o desvio padrão com o buffer atual
      float normalizedValue = (filteredValue - mean) / (sd + 0.0001); // Evita divisão por zero
      Serial.println(normalizedValue);
    }
  }
}
