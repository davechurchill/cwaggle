#!/usr/bin/python

import sys
import os
import fileinput
import io

class ConfigEditor:
    def __init__(self, file):
        self.fileName = file
        self.width = 15
    
    def getSpacing(self, varName, value):
        while (len(varName) < self.width):
            varName = varName + ' '
        varName = varName + value + '\n'
        return varName

    def editValue(self, varName, value):
        # keeping proper spacing within the config files
        varName_orig = varName
        while (len(varName) < self.width):
            varName = varName + ' '

        self.file = open(self.fileName, "r+")
        lines = self.file.readlines() # saving the data from the original file
        self.file.close() # erasing the old file
        self.file = open(self.fileName, "r+")
        for line in lines:
            if line.strip().startswith(varName_orig):
                line = varName + value + '\n'
                self.file.write(line)
            else:
                self.file.write(line)
        self.file.close()

    def writeNewFile(self):
        self.file = open(self.fileName, "r+")
        self.file.truncate()
        self.file.write(self.getSpacing('width', '800'))
        self.file.write(self.getSpacing('height', '800'))
        self.file.write(self.getSpacing('numRobots', '1'))
        self.file.write(self.getSpacing('robotRadius', '10'))
        self.file.write(self.getSpacing('gui', '1'))
        self.file.write(self.getSpacing('numPucks', '250'))
        self.file.write(self.getSpacing('puckRadius', '10'))
        self.file.write(self.getSpacing('simTimeStep', '1'))
        self.file.write(self.getSpacing('renderSkip', '1000'))
        self.file.write(self.getSpacing('forwardSpeed', '2.0'))
        self.file.write(self.getSpacing('angularSpeed', '0.3'))
        self.file.write(self.getSpacing('outieThreshold', '0.6'))
        self.file.write(self.getSpacing('innieThreshold', '0.8'))
        self.file.write(self.getSpacing('hashFunction', 'Original'))
        self.file.write(self.getSpacing('actions', '4 0.3 0.15 -0.15 -0.3'))
        self.file.write(self.getSpacing('maxTimeSteps', '2000000'))
        self.file.write(self.getSpacing('batchSize', '1'))
        self.file.write(self.getSpacing('initialQ', '1.0'))
        self.file.write(self.getSpacing('alpha', '0.2'))
        self.file.write(self.getSpacing('gamma', '0.9'))
        self.file.write(self.getSpacing('epsilon', '0.0'))
        self.file.write(self.getSpacing('resetEval', '0.94'))
        self.file.write(self.getSpacing('writePlotSkip', '0'))
        self.file.write(self.getSpacing('plotFilename', 'gnuplot/1.txt'))
        self.file.write(self.getSpacing('qLearning', '1'))
        self.file.write(self.getSpacing('savePolicy', '100000 gnuplot/q_out.txt'))
        self.file.write(self.getSpacing('loadPolicy', '0 gnuplot/q_out.txt'))
        self.file.write(self.getSpacing('resultsDir', 'results/exp1/'))
        self.file.write(self.getSpacing('importGrid', '0 images/potential_field_L.png'))
        self.file.write(self.getSpacing('RLAction', '0'))
        self.file.close()