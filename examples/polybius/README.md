# Polybius

!!!!!! CUIDADO: Pode causar epilepsia !!!!!!

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

Implementação de uma página de descanso de um computador.
O objeto anda linearmente até um dos cantos, ao bater nesse canto, o objeto é trocado por outro e as cores do fundo são alternadas randomicamente.

## Sobre a implementação:

O programa foi implementado com base na aplicação [starfield](https://hbatagelo.github.io/cg/starfield.html) desenvolvida pelo Professor Harlen Batagelo para o curso MCTA008-17 Computação Gráfica na UFABC, utilizando a biblioteca ABCg e o pipeline do OpenGL.
O objetivo principal da nossa nova aplicação é o objeto andar pela tela e, ao bater em um dos cantos, o objeto alternar e as cores do fundo também.
O objeto alterna randomicamente as suas próprias cores com o passar do tempo.
Então, de objetos temos:

- Cubo;
- Prisma;
- Esfera;

Realizamos a adição de implementação no `openglwindow`, para adicionar os .obj alternadamente, além de realizar updates nesse mesmo objeto (rotacionar em seu próprio eixo, andar para frente, alterar a rota caso haja colisão com o canto da tela e alternar sua própria cor com o passar do tempo).
Outro ponto importante é a alteração do background de acordo com as colisões, sendo gerado uma nova cor randomicamente.

### Inspiração

Esse projeto tem como inspiração uma lenda urbana de um jogo arcade chamado Polybius, era dito que esse jogo era utilizado como um experimento psicológico realizado pelo governo dos EUA.
O jogo supostamente produzia um efeito psicodélico intenso e viciante nos jogadores.

Implementamos um protótipo mais leve do mesmo.

[Vídeo explicando o Polybius](https://www.youtube.com/watch?v=hyNWn8VD3eE)
