# Cor
- Noção perceptual
- Interpretação dada pelo cérebro humano para ondas eletromagnéticas

## Distribuição Espectral
- Uma *fonte de luz* comumente emite *simultaneamente* ondas de vários comprimentos de onda
- A *cor vista de um objeto* depende da:
	- Distribuição espectral da *fonte de luz*
	- *Refletância* de sua superfície
		- Função que determina o percentual da luz incidente que é refletida **para cada comprimento de onda**
- Queremos simular este processo em computação gráfica
- *Três números* são, em princípio, suficientes para determinar uma cor *para o sistema visual humano*; **Note a diferença:** três números descrevem todas as cores que "conhecemos", mas não são suficientes para determinar qualquer distribuição espectral
	- Existem distribuições espectrais *diferentes* que o sistema visual humano *percebe* como sendo *a mesma cor*!
	
## Modelo de cores RGB
- Representa cada cor C através de três números reais (r,g,b) no intervalo [0,1]
- Estes números determinam uma *combinação linear de três cores primárias*: R, G, B
	- `C = rR + gG + bB;`
- Existem várias opções para cores primárias
	- Pois existem vários tons de vermelho, verde e azul
- Para cada escolha de primárias RGB, temos um **espaço de cor** RGB diferente
	- Existem vários padrões internacional
		- sRGB (mais comum), CIE-RGB, Adobe RGB...
		
## Em OpenGL:

- No framebuffer, os valores de r, g, b comumente são representados c/ 8 bits real ∈ [0,1] -> inteiro ∈ [0,255]
```
out vec3 color;

void main()
{
	color = vec(1.0, 0.0, 0.0); // r, g, b
}
```

# RGBA: (r,g,b,a)
- Adiciona-se mais um componente
- Alpha: componente de transparência
- Para habilitar Alpha Compositing em OpenGL:
```
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
```
