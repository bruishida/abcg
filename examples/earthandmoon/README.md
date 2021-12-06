# Earth And Moon

## Indice:

- [Autoria](##_Autoria)
- [Introdução](##_Introdução)
- [Sobre a implementação](##_Sobre_a_implementação)
- [Inspiração](##_Inspiração)

---

## Autoria:

- Bruna Ishida - 11201722024
- Nicolle Ayumi Adaniya - 110201721976

## Introdução:

Implementação do sistema entre a Terra e a Lua, demonstrando suas rotações e luminosidades em relação ao Sol.
O usuário podendo navegar no sistema por meio dos comandos do teclado.

## Sobre a implementação:

O programa foi implementado com base na aplicação [lookat](https://hbatagelo.github.io/cg/lookatproject.html) desenvolvida pelo Professor Harlen Batagelo para o curso MCTA008-17 Computação Gráfica na UFABC, utilizando a biblioteca ABCg e o pipeline do OpenGL.

O objetivo principal da nossa nova aplicação é apresentar a rotação e luminosidade da Terra e da Lua em relação ao Sol.

Para movimentar a câmera, utilize o teclado:

- W ou Seta para cima: aproxima a câmera;
- A ou Seta para a esquerda: movimenta a câmera para a esquerda;
- S ou Seta para a direita: movimenta a câmera para a direita;
- D ou Seta para baixo: desaproxima a câmera;
- I ou Seta para baixo: sobe a câmera;
- K ou Seta para baixo: desce a câmera.

Realizamos a adição de implementação no `openglwindow`, para a movimentação da câmera por meio de eventos no teclado, incluindo um novo modelo de câmera denominado `plane` que consiste em movimentá-la no eixo Y, além de adicionar os dois objetos com suas respectivas texturas.
Os objetos possuem rotatividade e tamanhos de acordo com as escalas entre a Terra e a Lua.

Utilizou-se o shader texture para a realização da animação na rotação com a luminosidade em um lado apenas de ambos os objetos, simulando a iluminação provinda do Sol.

## Inspiração

Não houveram inspirações dessa vez :(
