import os, re

def run_tests(n):
    for i in range(n):
        op = list()
        # os.system(f"./test < input/{6}.txt")
        os.system(f"./test < input/{i}.txt > output/{i}.txt")
        # quit()
        with open(f"output/{i}.txt") as f:
            lst = f.readlines()
            for line in lst:
                op.append(line.strip())
        with open(f"input/{i}.txt") as f:
            lst = f.readlines()
            text = lst[0].strip()
            M = int(lst[1].strip())
            for j in range(M):
                regex = lst[2+j].strip()
                res = re.search(regex, text)
                if res == None:
                    ans = "0"
                else:
                    spn = list(res.span(0))
                    spn[1] -= 1
                    if spn[0] < 0:
                        spn[0] = 0
                    if spn[1] < 0:
                        spn[1] = 0
                    ans = f"1 {spn[0]} {spn[1]}"
                if ans == op[j]:
                    print(f"Testcase {i} regex {j+1} " + '\x1b[6;30;42m' + " PASSED " + '\x1b[0m')
                else:
                    print(f"Testcase {i} regex {j+1} " + '\x1b[6;30;41m' + " FAILED " + '\x1b[0m' + " Text = " + text + " Regex = " + regex + " Expected answer = " + ans + " Your answer = " + op[j])

def main():
    print("Add your file in the A3_tests folder.")
    srn = input("Enter SRN: ")
    file_name = "A3_" + srn
    file_name = file_name.upper() + ".c"

    os.system(f"gcc -Wall {file_name} -o test")

    path, dirs, files = next(os.walk("input"))
    run_tests(len(files))

    print("You can go and check your outputs in the output folder.")


if __name__ == '__main__':
    main()