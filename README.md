# Jogo do Pato

<p align="center">
 <a href="## Autoria">Autoria</a> • <a href="## Como jogar">Como jogar</a> • <a href="## Objetivo do jogo">Objetivo do jogo</a> •  <a href="## Sobre a implementação">Sobre a implementação</a> •  <a href="### Classes ">Classes</a> • <a href="### Inspiração">Inspiração</a> • <a href="### Créditos pelo ABCg">Créditos pelo ABCg</a> •  <a href="### Licença">Licença</a>
</p>

---

## Autoria:

- Bruna Ishida - 11201722024
- Nicolle Ayumi Adaniya - 110201721976

## Como jogar:

Para movimentar o pato, utilize as teclas WASD ou as setas no teclado numérico.

- W ou Seta para cima: movimenta o pato para cima;
- A ou Seta para a esquerda: movimenta o pato para o lado esquerdo;
- S ou Seta para a direita: movimenta o pato para o lado direito;
- D ou Seta para baixo: movimenta o pato para baixo.

Ao abrir o link do jogo, ele iniciará automaticamente. Em caso de vitória ou derrota, o jogo iniciará novamente em alguns segundos.

## Objetivo do jogo:

Para ganhar o jogo, você deve controlar o pato para desviá-lo das bombas que estão caindo durante 10 segundos. Caso alguma bomba atinja o pato, você perde.

## Sobre a implementação:

O jogo foi implementado com base na aplicação [asteroids](https://hbatagelo.github.io/abcgapps/asteroids/ "asteroids") desenvolvida pelo Professor Harlen Batagelo para o curso MCTA008-17 Computação Gráfica na UFABC, utilizando a biblioteca ABCg e o pipeline do OpenGL.
O objeto principal do programa é desenvolver uma aplicação interativa com gráficos 2D. Desse modo, o Jogo do Pato tem como objetos:

- Um pato, construído através de `GL_TRIANGLES`;
- Bombas, construídos através de `GL_TRIANGLE_FAN`.

### Classes

Para organizar o projeto, separamos as implementações em:

- `OpenGlWindow`: responsável pela integração do componentes e realização dos fluxos do jogo;
- `Duck`: responsável pelo "jogador" que será um pato, com VAO, VBO e atributos do objeto;
- `Bombs`: responsável pela geração das bombas e suas dinâmicas, com inspiração oriunda da aplicação mencionada acima;
- `GameData`: comandos existentes nas regras do jogo e os steps do mesmo.

### Inspiração

O jogo foi inspirado em uma propaganda dos EUA na época da Guerra Fria. A propaganda tinha como objetivo conscientizar as pessoas e educá-las a como reagirem em casos de uma bomba ser explodida por perto.
O pato, então, representa a maneira de se portar com uma bomba no começo do anúncio.

[Vídeo da propaganda](https://www.youtube.com/watch?v=IKqXu-5jw60)

---

### Créditos pelo ABCg

Desenvolvido por Harlen Batagelo.

### Licença

ABCg é licenciado sob o MIT License. Veja [LICENÇA](https://github.com/hbatagelo/abcg/blob/main/LICENSE) para mais informações.
