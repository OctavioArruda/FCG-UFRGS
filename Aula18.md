# Modelos de Iluminação 

## Iluminação real:
- A cor que percebemos para cada ponto da superficie de um objeto depende de:
	- Propriedades espectrais da superfície
	- Propriedades espectrais da fonte de luz
	- Relação geométrica entre objeto e fonte de luz
	- Relação geométrica entre objetos  
	
## Tipos de Rendering
- *Photorealistic Rendering*
	- Visa simular o processo de iluminação real
	- Problema computacionalmente complexo
	- Utiliza-se simplificações
- *Non-Photorealistic Rendering(NPR)*
	- Visa outros objetivos artísticos e técnicos, como simular pinturas e cartoons
	
## Shading
- Em CG, o processo de determinação da cor de um pixel é genericamente chamado de **shading**
- **Modelos de iluminação** são também chamados de **shading models** ou **modelos de sombreamento**

## Propriedades Espectrais
- Valores RGB associados a iluminação refletidas de um certo ponto de algum objeto **não possuem limite superior**
	- Problema: dispositivos rgb aceitam no máximo valores RGB até 1
	- Solução: tone mapping -> Remapear valores fora do intervalo para o intervalo de 0 a 1
		- Clamping

## Propriedades Geométrica

### Reflexão Especular(ideal)
	- Propaga o raio de luz de modo que o ângulo de reflexão seja igual ao ângulo de incidência 
	- Espelho ideal
![alt text](https://i.imgur.com/xNO9WWL.png)

### Reflexão Especular Glossy
	- Iluminação incidente é espalhada em torno da direção de reflexão ideal
	
### Reflexão difusa(ideal)
	- Radiância igual em todas as direções do hemisfério definido pela normal
	
	
## Shading interpolation models

### Flat Shading
- Equação de iluminação computada uma vez para cada face (triângulo) do modelo
- Como obter vetores normais por triângulo? Produto vetorial

### Gourad Shading
- Equação de iluminação computada uma vez para cada vértice do modelo poligonal
- Resultado é interpolado para cada pixel pelo rasterizador
- Funciona bem para superfícies difusas
- Como obter vetores normais por vértice? Analiticamente ou média
	- Vertex models: se conhecemos a superfície original, podemos computar as normais analiticamente através do calculo vetorial
	- Se não conhecemos a superfície original, podemos usar o método Gouraud:
		- A normal de cada vértice será dada pela média das normais de cada triângulo que compartilha o vértice em questão. Sempre funciona? Não para cubos: os vértices de um cubo são pontos não-diferenciaveis, e portanto não possuem normais bem definidas
		- Premissa: superfície suave(campo vetor de normais contínuo)

### Phong Shading
- Equação de iluminação computada uma vez para cada pixel gerado pelo rasterizador
- Como obter vetores normais por pixel?
- Como obter posiçaõ global por pixel?
- Resposta: estes são interpolados pelo rasterizador a partir dos atributos de cada vértice -> posição, normal, cor, etc
- Custo computacional elevado vs Gourad
	- Interpolação, e #pixels >> #vértices

### Fontes de luz
- Pontual: emite iluminação igualmente para todas as direções
	- Parâmetros: posição l
- Spotlight: emite iluminação somente dentro de um cone
	- Parâmetros: posição l, direção vec v, abertura alfa(ângulo)
