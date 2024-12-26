import os
import subprocess
from termcolor import colored   # pip install termcolor

EXECUTAVEL = "./tp2"
INPUT = "testCases/inputs"
OUTPUT = "testCases/outputs"
os.system('clear')

def run_test(input_file, expected_output_file):
    # Abre os arquivos de entrada e saída esperada
    with open(input_file, "r") as infile, open(expected_output_file, "r") as expected_file:
        expected_output = expected_file.read().strip()

        # Executa o programa usando o arquivo de entrada como entrada padrão
        result = subprocess.run(
            EXECUTAVEL, stdin=infile, stdout=subprocess.PIPE, text=True
        )
        actual_output = result.stdout.strip()

        # Compara a saída gerada com a saída esperada
        if actual_output == expected_output:
            return True, None
        else:
            return False, actual_output

def main():
    input_files = sorted(os.listdir(INPUT))
    passed = 0

    for input_file in input_files:
        # Extrai o número do teste a partir do nome do arquivo
        test_number = input_file.replace("testCase", "").replace(".txt", "")

        # Determina o caminho do arquivo de saída esperada correspondente
        expected_output_file = os.path.join(OUTPUT, f"testCase{test_number}.txt")
        input_file_path = os.path.join(INPUT, input_file)

        # Verifica se o arquivo de saída esperada existe
        if not os.path.exists(expected_output_file):
            print(f"⚠️ Saída esperada para {input_file} não encontrada.")
            continue

        # Executa o teste
        success, actual_output = run_test(input_file_path, expected_output_file)

        # Exibe os resultados do teste
        if success:
            message = colored(f"Teste {test_number} passou.", 'light_green', attrs=['bold'])
            print(message)
            passed += 1
        else:
            print(colored(f"Teste {test_number} falhou.", 'red', attrs=['bold']))
            print(colored(f"Saída esperada", attrs=['bold', 'underline']), end=':\n')
            print(f'{open(expected_output_file).read().strip()}')
            print(colored(f"Saída obtida", attrs=['bold', 'underline']), end=':\n')
            print(f'{actual_output}')
            print('-'*20)
    total_tests = len(input_files)
    print(colored(f"\n{passed}/{total_tests} testes passaram.", 'yellow', attrs=['bold']))

if __name__ == "__main__":
    main()