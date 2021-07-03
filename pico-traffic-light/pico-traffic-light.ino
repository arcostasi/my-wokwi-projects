#define LED_RED_1    2
#define LED_YELLOW_1 3
#define LED_GREEN_1  4

#define LED_RED_2    8
#define LED_YELLOW_2 9
#define LED_GREEN_2  10

int main() {
    // Define as portas como saída
    _gpio_init(LED_RED_1);
    gpio_set_dir(LED_RED_1, GPIO_OUT);

    _gpio_init(LED_YELLOW_1);
    gpio_set_dir(LED_YELLOW_1, GPIO_OUT);

    _gpio_init(LED_GREEN_1);
    gpio_set_dir(LED_GREEN_1, GPIO_OUT);

    _gpio_init(LED_RED_2);
    gpio_set_dir(LED_RED_2, GPIO_OUT);

    _gpio_init(LED_YELLOW_2);
    gpio_set_dir(LED_YELLOW_2, GPIO_OUT);

    _gpio_init(LED_GREEN_2);
    gpio_set_dir(LED_GREEN_2, GPIO_OUT);

    while (1) {
      // Liga o vermelho dos dois semáforos
      gpio_put(LED_RED_1, 1);
      gpio_put(LED_YELLOW_1, 0);
      gpio_put(LED_GREEN_1, 0);

      gpio_put(LED_RED_2, 1);
      gpio_put(LED_YELLOW_2, 0);
      gpio_put(LED_GREEN_2, 0);

      // Esperamos 1s com o sinal no vermelho
      sleep_ms(1000);

      // Desliga o vermelho do semáforo 1
      gpio_put(LED_RED_1, 0);

      // Liga o verde do semáforo 1
      gpio_put(LED_GREEN_1, 1);

      // Esperamos 3s com o sinal aberto
      sleep_ms(3000);

      // Apagamos o verde e acendemos o amarelo
      gpio_put(LED_GREEN_1, 0);
      gpio_put(LED_YELLOW_1, 1);

      // Esperamos 1s com o sinal no amarelo
      sleep_ms(1000);

      // Acende o vermelho dos dois semáforos
      gpio_put(LED_YELLOW_1, 0);
      gpio_put(LED_RED_1, 1);
      gpio_put(LED_RED_2, 1);

      // Esperamos 1s com o sinal fechado
      sleep_ms(1000);

      // Desliga o vermelho do semáforo 2
      gpio_put(LED_RED_2, 0);

      // Liga o verde do semáforo 2
      gpio_put(LED_GREEN_2, 1);

      // Esperamos 3s com o sinal aberto
      sleep_ms(3000);

      // Apagamos o verde e acendemos o amarelo
      gpio_put(LED_GREEN_2, 0);
      gpio_put(LED_YELLOW_2, 1);

      // Esperamos 1s com o sinal amerelo
      sleep_ms(1000);
    }

    return (0);
}
