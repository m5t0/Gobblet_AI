from test import support
import neural_network_test

def main():
    modules = ['neural_network_test']
    for m in modules:
        print(f'test module:{m}')
        print()
        support.run_unittest(m)
        print()
        print('*'*70)
        print()


if __name__ == "__main__":
    main()
