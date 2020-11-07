Iluminação global

Rasterização:
- Uso de memória: imagem sendo gerada
- Um objeto por vez
- Depth complexity com overdraw(mesmo pixel sendo escrito várias vezes)
- Iluminação & outros: difícil simular interação luminosa entre objeto
- Rendering eficiente para cenas com muitos objetos

Ray Casting:
- Uso de memória: imagem sendo gerada
- Todos os objetos
- Depth complexity sem overdraw
- Iluminação & outros: fácil simular interação luminosa entre objeto
- Obrigatório uso de estruturas de dados de aceleração para os testes de intersecção em cenas com muitos objetos

- Se utilizarmos modelos de iluminação *local*, rasterização e ray casting iraõ gerar a mesma imagem final

Recursive Ray Tracing:
- Quando um raio atingir uma superfície, geramos outro(s) raio(s) recursivamente
	- Sombras, reflexão e transmissão especular *ideal*
	
