# Aula 16: Curvas e Superfícies

## Curvas paramétricas:
- Conjunto de pontos {c(t) | t ∈ [ t0, t1]}
- Cada ponto c(t) associado a um valor do parâmetro real t
- Como que um usuário pode *construir uma curva* de seus interesse?
	- Especificar as funções Cx(t), Cy(t),...
	- Ideia básica: **Pontos de controle**
	
## Pontos de controle:
- Pontos p1, p2, ..., pn (definidos pelo usuário)
- Curva interpolante: passa por todos pi
- Curva aproximada: não passa por todos pi
- Se conectarmos os pontos de controle, em ordem,definidos algo que chamamos de polígono de controle da curva
- Círculos também são definidos com curvas de Bézier 

## Curvas de Bézier:
- Grau (degree) = número de pontos - 1
- Passa exatamente por cima do primeiro e último ponto, entretanto, não passam necessariamente pelos outros pontos
- Sempre contida no convex-hull (polígono de controle)
- Invariante a transformações afins (translação, rotação, etc)
	- Isto é, para transformar a curva, é somente necessário transformar os pontos de controle
- Uma curva de Bézier de grau 1 nada mais é que uma reta entre seus dois pontos de controle


- c(t) = p1 + t(p2 - p1) onde t ∈ [0, 1]	

- Uma curva de Bézier de grau 2 possui três pontos de controle:
	- c12(t) = p1 + t(p2 - p1); `Interpolação linear entre pontos 1 e 2`
	- c23(t) = p2 + t(p3 - p2); `Interpolação linear entre pontos 2 e 3`
	- c(t) = c12(t) + t(c23(t) - c12(t)); `Curva em si`
	
- Caso seja expandido c(t), acima, teremos:
	- c(t) = p1 + t(2p2 - 2p1) + t²(p1 - 2p2 + p3); `Rearrajando, temos:`
	- c(t) = (1 - t)²p1 + 2t(1 - t)p2 + t²p3; `Polinômios de Bernstein de grau 2`
	
- Uma curva de Bézier de grau 3 possui três pontos de controle:
	- c12(t) = p1 + t(p2 - p1); `Interpolação linear entre pontos 1 e 2`
	- c23(t) = p2 + t(p3 - p2); `Interpolação linear entre pontos 2 e 3`
	- c34(t) = p3 + t(p4 - p3); `Interpolação linear entre pontos 3 e 4`
	- c123(t) = c12(t) + t(c23(t) - c12(t)); `Interpolação linear entre os pontos gerados acima`
	- c234(t) = c23(t) + t(c34(t) - c23(t));
	- c(t) = c123(t) + t(c234(t) - c123(t));  `Curva em si`

- De maneira geral, uma curva de Bézier de Grau n possui n + 1 pontos de controle
- Normalmente utilizamos curvas de Bézier com grau <= 3
- Desvantagem de uma curva de grau elevado: 
	- Ao modificar qualquer ponto *toda a curva* será (levemente) modificada
- Possível solução para curvas de grau elevado: **Piecewewise Cubic Bézier Curves**
	- Várias peças de curvas de Bézier cúbicas conectadas
	- Vantagens: *controle facilitado e local*
	- Cuidados: Possível descontinuidade nos pontos de controle que são compartilhados entre as partes da curva
- **Cubic Hermite Splines:** construção alternativa para Piecewise Cubic Bézier 
	- Piecewise Cubic Bézier:
		- Definida por pontos (de controle)
		- Interpola 1 a cada 3 pontos de controle
	- Cubic Hermite Splines:
		- Definida por pontos + vetores tangentes
		- Interpola todos os pontos
		- Usuário precisa definir os vetores tangentes!
	- Pode-se facilmente converter uma curva no formato de Piecewise para Cubic Hermite Splines
- **Catmull-Rom Splines:**
	- São curvas Cubic Hermite Splines onde as tangentes são calculadas a partir dos pontos
	
## Superfícies Paramétricas
- Curva = interpolação de pontos; Superfície = interpolação de curvas
- Cada ponto c(t, s) associado a um valor do parâmetro t e um valor do parâmetro s
	

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
