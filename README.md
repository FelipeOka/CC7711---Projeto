# CC7711 - Inteligência Artificial e Robótica - Projeto Final

### Integrantes:

- Ana Jéssica Soares da Silva <br> R.A: 22.121.100-6
- Eduardo Antunes Vicente <br> R.A: 22.121.010-7
- Fabio Augusto Schiavi Morpanini <br> R.A: 22.121.094-1
- Felipe de Campos Oka <br> R.A: 22.121.001–6

#### Objetivo:
- Encontrar a caixa de madeira “leve”

#### Observações:
- O robô deve encontrar a caixa leve e parar de procurar <br>
- Quando encontrar, ele deve parar e acender os leds

#### Desenvolvimento:

Inicialmente, objetivamos focar nossos esforços na detecção do arrasto da caixa quando o robô “e-puck” colidisse com ela, contudo, tal método se mostrou muito complexo e ineficiente, uma vez que o professor havia dito que realizaria testes do mesmo robô em outros mundos diferentes. Em suma, nos foi orientado a não focar no arrasto das caixas, mas sim mapeá-las de modo que, o robô “soubesse” qual é a caixa leve, e assim que colidisse com a mesma, os led’s acenderiam e o robô pararia de se mover.
	Deste modo, definimos uma variável “caixaLeve” ao objeto da caixa mais leve e a referenciamos no controller do robô, para isto, utilizamos principalmente da função “supervisor” disponível na documentação do site Webots. A partir disso, fomos capazes de adquirir a posição de ambas as caixas quanto o robô

#### Conclusão:

Para que funcionasse corretamente, foi necessário adicionar a variável “caixaLeve” ao controlador do e-puck e habilitar a função supervisor disponível no e-puck. A principal dificuldade encontrada, se refere ao acender dos leds quando o robô encontrasse a caixa leve, tal dificuldade, se deu pelo fato da luminosidade do mundo requisitado para entrega do projeto, ser mais claro com relação ao disponibilizado para desenvolvimento, para corrigir este problema, foi necessário diminuir a luminosidade, fazendo com que se tornasse visível o funcionamento dos leds.

Foram realizadas 6 simulações, em ambos os mundos disponibilizados (2 no mundo para testes e 4 no mundo para envio), de modo a demonstrar a eficiência do robô em reconhecer os objetos “pesados” e encontrar a caixa leve.

#### Vídeo <br>
https://drive.google.com/file/d/1CTeirYRPCQwJuGsrbTZwWEoGTN1FB5lf/view?usp=drive_link

### Observação: é necessário abrir o arquivo "controller" pelo WeBots e salvá-lo na pasta onde ficam armazenados os Controllers na máquina
