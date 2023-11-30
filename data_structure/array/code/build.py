#!/usr/bin/env python3
import os
import subprocess

if not os.path.exists('build'):
    os.makedirs('build')

os.chdir('build')

subprocess.run(['cmake', '..'])

subprocess.run(['make'])

print("Configuração concluída. Agora você pode compilar o projeto.")
