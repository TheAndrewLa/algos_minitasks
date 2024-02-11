import subprocess
import os
import sys

# checking that we're currently at the git repo folder
if not ('.git' in os.listdir(os.getcwd())):
    print("Bad directory to perform git bisect")
    exit(1)

# getting command-line args (first and last commits, commands to run and arguments to command)
first_commit = sys.argv[1]
last_commit = sys.argv[2]

command = sys.argv[3]
args = sys.argv[4:]

# functions that call our command on excact commit (checkout on needed commit, run checking, checkout to current files)
def check_commit(commit) -> bool:
    subprocess.run(['git', 'checkout', commit])
    code = subprocess.run([command, args]).returncode
    subprocess.run(['git', 'checkout'])
    return code == 0

# getting list of all commits on repository
# reversing list of all commits
# getting hash of each commit in the list
commits = subprocess.run(['git', 'log', '--oneline', '--no-abbrev-commit'], capture_output=True).stdout
commits = commits.splitlines()[::-1]
commits = [i.decode("utf-8").split()[0] for i in commits]

# getting an actual commit (to perform git checkout at the end)
actual_commit = commits[-1]

while commits[0] != first_commit:
    commits.pop(0)

while commits[-1] != last_commit:
    commits.pop()

if not (check_commit(first_commit) and not check_commit(last_commit)):
    print("Bad commits has been provided to the script")
    exit(1)
    
left, right = 0, len(commits) - 1
while left < right:
    pivot = (left + right) // 2
    
    if check_commit(commits[pivot]):
        left = pivot + 1
    else:
        right = pivot - 1

# Printing the hash of first bad commit in the sequence
print(f"The first bad commit has this hash: {commits[left]}")

# Checkout to last commit
subprocess.run(['git', 'checkout'])
