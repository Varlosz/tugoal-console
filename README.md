# Readme TuGoal

**TuGoal** é uma aplicação desenvolvida em linguagem C para a gestão completa de um campeonato de futebol. O sistema permite administrar equipas, jogos e classificações de forma dinâmica, garantindo a persistência de dados e uma interface de consola interativa e colorida.

Este projeto foi criado no âmbito da unidade curricular de Programação (PIP), com o objetivo de estruturar e gerir uma época desportiva virtual. O foco principal foi garantir a consistência dos dados, evitar erros de utilização e proporcionar uma simulação realista das jornadas.

### Funcionalidades Principais

#### 1. Gestão de Equipas
* **Criação e Edição:** Adicionar novas equipas (com verificação de nomes duplicados) e editar nomes existentes.
* **Segurança:** Bloqueio de eliminação de equipas que já tenham jogos realizados para manter a integridade do histórico.
* **Listagem Organizada:** Visualização de equipas ordenadas por ID para fácil seleção.

#### 2. Gestão de Jogos e Resultados
* **Registo Manual:** Interface visual "Versus" (Equipa A vs Equipa B) para inserção de resultados, com validação de dados (impede golos negativos).
* **Simulação de Jornadas:** Motor de simulação que gera resultados aleatórios para uma jornada inteira, baralhando os jogos para maior realismo.
* **Histórico:** Consulta de todos os jogos realizados, organizados por jornadas e com indicação visual de cores para vitórias, empates e derrotas.

#### 3. Classificação e Estatísticas
* **Tabela Dinâmica:** Classificação atualizada automaticamente após cada jogo.
* **Critérios de Desempate Oficiais:**
    1. Pontos
    2. Diferença de Golos (DG)
    3. Golos Marcados (Melhor Ataque)
* **Visualização:** Cores indicativas para zonas de campeão (Champions), competições europeias e zona de descida.

### Melhorias Técnicas e Correções

Baseado no changelog recente, o projeto inclui várias otimizações:

* **Persistência de Dados:** Todos os dados (equipas e jogos) são gravados automaticamente em ficheiros (`.txt`), prevenindo perda de informação ao fechar o programa.
* **Estabilidade:** Correção de erros críticos, como *crashes* ao inserir letras em campos numéricos ou inconsistências nos ficheiros.
* **Organização de Código:** Estrutura modular separada em ficheiros de cabeçalho (`.h`) e código fonte (`.c`) para facilitar a manutenção.
* **Interface:** Menus consistentes (0 para voltar/sair) e tabelas alinhadas independentemente do tamanho dos nomes das equipas.