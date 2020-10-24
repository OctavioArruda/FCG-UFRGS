# Mapeamento de Texturas

## UV Unwrapping

- Muito utilizado para UV mapping
- Ideia: "desembrulhar" a superfície, cortando e esticando até a mesma estar contida em um plano

## Como armazenar (u, v) para um modelo geométrico?

- Cada vértice contém atributos (u, v)

## Textura Wrapping Mode

- GL_REPEAT
- GL_MIRRORED_REPEAT
- GL_CLAMP_TO_EDGE
- GL_CLAMP_TO_BORDER

## Texture Mapping Problem

- Como obter coordenadas (u, v) para todo ponto p?

### Projeção planar:

- Coordenadas do objeto, ponto p (x, y, z)
- Coordenadas de textura (u, v)
	- Projeção planar:
		- (u, v) = (x, z)
- Fácil implementação, porém gera **distorções na textura** projetada em faces não-perpendiculares à direção de projeção
- Não é injetiva e **textura é repetida** em pontos diferentes do objeto

### Cube Projection Mapping

- Projeção de texturas diferentes por **seis lados** do objeto 

### Esfera: Mapeamento de textura

- Parametrização em ρ, θ e φ
- Mapear texturas em uma esfera é simples. Como utilizar isso para outros objetos?

### Cilindro: Mapeamento de textura

- Parametrização: ângulo θ, altura h e raio ρ











