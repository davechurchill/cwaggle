from ConfigEditor import *
import os

'''
Description: Experiment 1 - Comparing original OC algorithm with RL
'''
file1 = 'rl_config1.txt'
file2 = 'rl_config2.txt'
file3 = 'rl_config3.txt'
file4 = 'rl_config4.txt'
file5 = 'rl_config5.txt'
file6 = 'rl_config6.txt'
file7 = 'rl_config7.txt'
file8 = 'rl_config8.txt'
file9 = 'rl_config9.txt'
file10 = 'rl_config10.txt'

os.system("touch " + file1)
os.system("touch " + file2)
os.system("touch " + file3)
os.system("touch " + file4)
os.system("touch " + file5)
os.system("touch " + file6)
os.system("touch " + file7)
os.system("touch " + file8)
os.system("touch " + file9)
os.system("touch " + file10)

exp1 = ConfigEditor(file1)
exp2 = ConfigEditor(file2)
exp3 = ConfigEditor(file3)
exp4 = ConfigEditor(file4)
exp5 = ConfigEditor(file5)
exp6 = ConfigEditor(file6)
exp7 = ConfigEditor(file7)
exp8 = ConfigEditor(file8)
exp9 = ConfigEditor(file9)
exp10 = ConfigEditor(file10)

exp1.writeNewFile()
exp2.writeNewFile()
exp3.writeNewFile()
exp4.writeNewFile()
exp5.writeNewFile()
exp6.writeNewFile()
exp7.writeNewFile()
exp8.writeNewFile()
exp9.writeNewFile()
exp10.writeNewFile()

# OC Learn------------------------------------------------------------

exp1.editValue('numRobots', '8')
exp1.editValue('RLAction', '0')
exp1.editValue('loadPolicy', '0 gnuplot/q_out.txt')
exp1.editValue('savePolicy', '100000 gnuplot/q_out1.txt')
exp1.editValue('qLearning', '0')
exp1.editValue('maxTimeSteps', '10000000')
exp1.editValue('resultsDir', 'results/OC/')
exp1.editValue('epsilon', '0.0')

# OC Learn------------------------------------------------------------

exp2.editValue('numRobots', '8')
exp2.editValue('RLAction', '0')
exp2.editValue('loadPolicy', '0 gnuplot/q_out.txt')
exp2.editValue('savePolicy', '100000 gnuplot/q_out2.txt')
exp2.editValue('qLearning', '0')
exp2.editValue('maxTimeSteps', '10000000')
exp2.editValue('resultsDir', 'results/OC/')
exp2.editValue('epsilon', '0.0')

# OC Learn------------------------------------------------------------

exp3.editValue('numRobots', '8')
exp3.editValue('RLAction', '0')
exp3.editValue('loadPolicy', '0 gnuplot/q_out.txt')
exp3.editValue('savePolicy', '100000 gnuplot/q_out3.txt')
exp3.editValue('qLearning', '0')
exp3.editValue('maxTimeSteps', '10000000')
exp3.editValue('resultsDir', 'results/OC/')
exp3.editValue('epsilon', '0.0')

# OC Learn------------------------------------------------------------

exp4.editValue('numRobots', '8')
exp4.editValue('RLAction', '0')
exp4.editValue('loadPolicy', '0 gnuplot/q_out.txt')
exp4.editValue('savePolicy', '100000 gnuplot/q_out4.txt')
exp4.editValue('qLearning', '0')
exp4.editValue('maxTimeSteps', '10000000')
exp4.editValue('resultsDir', 'results/OC/')
exp4.editValue('epsilon', '0.0')

# OC Learn------------------------------------------------------------

exp5.editValue('numRobots', '8')
exp5.editValue('RLAction', '0')
exp5.editValue('loadPolicy', '0 gnuplot/q_out.txt')
exp5.editValue('savePolicy', '100000 gnuplot/q_out5.txt')
exp5.editValue('qLearning', '0')
exp5.editValue('maxTimeSteps', '10000000')
exp5.editValue('resultsDir', 'results/OC/')
exp5.editValue('epsilon', '0.0')

# RL Learn------------------------------------------------------------

exp6.editValue('numRobots', '8')
exp6.editValue('qLearning', '1')
exp6.editValue('maxTimeSteps', '10000000')
exp6.editValue('loadPolicy', '0 gnuplot/q_out_rl.txt')
exp6.editValue('resultsDir', 'results/RL/')
exp6.editValue('epsilon', '0.00')
exp6.editValue('savePolicy', '100000 gnuplot/q_out6.txt')

# RL Learn------------------------------------------------------------

exp7.editValue('numRobots', '8')
exp7.editValue('qLearning', '1')
exp7.editValue('maxTimeSteps', '10000000')
exp7.editValue('loadPolicy', '0 gnuplot/q_out_rl.txt')
exp7.editValue('resultsDir', 'results/RL/')
exp7.editValue('epsilon', '0.00')
exp7.editValue('savePolicy', '100000 gnuplot/q_out7.txt')

# RL Learn------------------------------------------------------------

exp8.editValue('numRobots', '8')
exp8.editValue('qLearning', '1')
exp8.editValue('maxTimeSteps', '10000000')
exp8.editValue('loadPolicy', '0 gnuplot/q_out_rl.txt')
exp8.editValue('resultsDir', 'results/RL/')
exp8.editValue('epsilon', '0.00')
exp8.editValue('savePolicy', '100000 gnuplot/q_out8.txt')

# RL Learn------------------------------------------------------------

exp9.editValue('numRobots', '8')
exp9.editValue('qLearning', '1')
exp9.editValue('maxTimeSteps', '10000000')
exp9.editValue('loadPolicy', '0 gnuplot/q_out_rl.txt')
exp9.editValue('resultsDir', 'results/RL/')
exp9.editValue('epsilon', '0.00')
exp9.editValue('savePolicy', '100000 gnuplot/q_out9.txt')

# RL Learn------------------------------------------------------------

exp10.editValue('numRobots', '8')
exp10.editValue('qLearning', '1')
exp10.editValue('maxTimeSteps', '10000000')
exp10.editValue('loadPolicy', '0 gnuplot/q_out_10.txt')
exp10.editValue('resultsDir', 'results/RL/')
exp10.editValue('epsilon', '0.00')
exp10.editValue('savePolicy', '100000 gnuplot/q_out10.txt')


os.system("./cwaggle_rl rl_config1.txt & ./cwaggle_rl rl_config3.txt & ./cwaggle_rl rl_config5.txt & ./cwaggle_rl rl_config7.txt & ./cwaggle_rl rl_config9.txt")
os.system("./cwaggle_rl rl_config2.txt & ./cwaggle_rl rl_config4.txt & ./cwaggle_rl rl_config6.txt & ./cwaggle_rl rl_config8.txt & ./cwaggle_rl rl_config10.txt")