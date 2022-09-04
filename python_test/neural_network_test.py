import unittest

import os
import sys
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
sys.path.append(os.path.join(os.path.dirname(__file__), '..').join('python'))
import python

class NeuralNetworkTest(unittest.TestCase):
    def test_func(self):
        self.assertEqual(python.neural_network.func(), python.neural_network.func())

    def test_func2(self):
        self.assertEqual(python.neural_network.func(), python.neural_network.func())
