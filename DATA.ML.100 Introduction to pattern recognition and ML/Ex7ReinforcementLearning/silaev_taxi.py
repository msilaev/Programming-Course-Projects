# Load OpenAI Gym and other necessary packages

import warnings
import gym
import random
import numpy as np
import time

warnings.filterwarnings("ignore", category=DeprecationWarning) 


# Environment
env = gym.make("Taxi-v3", render_mode='ansi')

# Training parameters for Q learning
alpha = 0.9  # Learning rate
gamma = 0.9  # Future reward discount factor
epsilon = 0.1
action_size = 6
num_of_episodes = 1000
num_of_steps = 500  # per each episode

# Q tables for rewards
#Q_reward = -100000*numpy.ones((500,6)) # All same
Q_reward = -100000*np.random.random((500, 6))  # Random

# Training w/ random sampling of actions
# YOU WRITE YOUR CODE HERE

for episode in range(num_of_episodes):
    states = []
    actions = []
    rewards = []
    state = env.reset()[0]

    # Choose action (epsilon-greedy)
    if np.random.uniform() < epsilon:
        action = np.argmax(Q_reward[state, :])
    else:
        action = np.random.randint(0, action_size)
    
    # Run episode    
    for step in range(num_of_steps):        
        new_state, reward, done, truncated, info = env.step(action)
        # Choose action (epsilon-greedy)
        if np.random.uniform() < epsilon:
            new_action = np.argmax(Q_reward[state, :])
        else:
            new_action = np.random.randint(0, action_size)
        # Update rule
        if done:
            Q_reward[state, action] = reward 
            break
        else:
            Q_reward[state, action] = Q_reward[state, action] + alpha*(reward + gamma*np.max(Q_reward[new_state, :])-Q_reward[state, action]) 
            state = new_state
            action = new_action
    
###################################################

# Testing - "run your method 10 times"
num_of_tests = 10
max_time = 10000
revards_list = []
num_actions_list = []

# You can choose what number of episodes to run in each test. Some TAs said it should be num_episodes=1, others said 
# e.g. num_episodes=10*num_of_tests like in the lecture notebook: eval_policy(qtable_mihail,num_of_evals*10,100)
# I use the latter option. Please don't remove points if you disagree. 
#  
num_episodes = 10*num_of_tests

for _ in range(num_of_tests):
    num_of_actions = 0
    tot_reward = 0  

    for episodes in range(num_episodes):
    
        state = env.reset()[0]          
        done = False
    
        while (not done and num_of_actions < max_time):
            action = np.argmax(Q_reward[state, :])
            state, reward, done, truncated, info = env.step(action)
            tot_reward += reward
            num_of_actions += 1
    
    revards_list.append(tot_reward/num_episodes)
    num_actions_list.append(num_of_actions/num_episodes)

revards_list = np.array(revards_list)
num_actions_list = np.array(num_actions_list)

eval_reward_mean = np.mean(revards_list)
eval_num_actions_mean = np.mean(num_actions_list)

print(f"Average total reward is: {eval_reward_mean:.4f}")
print(f"Average number of actions: {eval_num_actions_mean:.4f}")     





