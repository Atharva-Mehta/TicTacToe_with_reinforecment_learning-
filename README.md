# Overview
This repository applies the reinforcement learning algorithm Q-learning to the simple pen and pencil
game cross & zero, also known as Tic-Tac-Toe. The goal is to analyze how different parameters
and opponents affect the performance of the Q-learning algorithm. Simple computer opponents
play against the Random Q-learning algorithm and later Q-learning algorithms higher level
experience. Several different parameters are tried and the results are plotted on detailed graphs.
The graphs show that the best results are gained against other Q-learning algorithms and a high
discount factor. When two Q-learning algorithms with different experiences played against each
other, the best results were obtained with a learning rate of 0.5. The best results were shown
when letting a Q-learning algorithm play against another Q-learning algorithm. A learning rate
and discount factor of 0.5 and 0.96 showed the best results, however the data suggested even
higher values of the discount factor might give even better results. In a Tic-Tac-Toe, it requires
about 1 million games to reach a good result.
