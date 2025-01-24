



# Edge case quoting and unclosed quotes
run_test 'echo test="1" test=1'
#!/bin/bash

# Path to your custom shell executable
MINISHELL=./minishell

# Function to execute commands in minishell and compare results with bash
run_test() {
	local command="$1"

	echo "Running test: $command"
	
	# Execute in minishell and extract just the output after prompt
	minishell_output=$(echo "$command" | $MINISHELL 2>&1 | grep -v "minishell> ")

	# Execute in bash 
	bash_output=$(echo "$command" | bash 2>&1)
	
	echo "Minishell Output: $minishell_output"
	echo "Bash Output: $bash_output"
	
	if [[ "$minishell_output" == "$bash_output" ]]; then
		echo "✅ Test passed: $command"
	else
		echo "❌ Test failed: $command"
		echo "  Expected: $bash_output" 
		echo "  Got: $minishell_output"
	fi
	echo
}

# Environment variable tests
run_test 'export test1="1" test2=2'
run_test 'env | grep test'

# Variable expansion tests
run_test 'echo $HOME'
run_test 'echo "$HOME"'
run_test "echo '$HOME'"

run_test 'echo $HOMEasdasd'
run_test 'echo asd$HOMEasdasd'
run_test 'echo "$HOMEasdasd"'
run_test 'echo "asd$HOMEasdasd"'
run_test 'echo "asd$HOMEasdasd lan"'
run_test 'echo asd$HOMEasdasd lan'
run_test "echo 'asd$HOMEasdasd lan'"

# Special variable tests
run_test 'echo $?'
run_test 'echo "$?"'
run_test '"echo '$?'"'

# Existing tests...
run_test 'export test1="1" test2=2'
run_test 'env | grep "test"'
run_test ''$HOME''
run_test '"$HOME"'
run_test '$HOME'

run_test '$?'
run_test '"$?"'
run_test ''$?''
run_test '$HOMEasdasd'
run_test 'echo $HOMEasdasd'
run_test 'echo asd$HOMEasdasd'
run_test '"$HOMEasdasd"'
run_test 'echo "$HOMEasdasd"'
run_test 'echo "asd$HOMEasdasd"'
run_test 'echo "asd$HOMEasdasd lan"'
run_test 'echo asd$HOMEasdasd lan'

run_test 'echo test="1" test=1'
run_test 'echo "test="1" test=1"'
run_test 'echo "test="1"" "test=1"'
run_test 'echo 'test="1"' 'test=1''
run_test 'echo 'test="1" test=1''

# Additional quote handling tests
run_test 'echo "Hello   World"'      # Multiple spaces in quotes
run_test 'echo "Hello""World"'       # Adjacent quotes
run_test "echo 'Hello'\"World\""     # Mixed quotes
run_test 'echo "$HOME$USER"'         # Multiple vars in quotes
run_test 'echo "$HOME""$USER"'       # Vars in separate quotes
run_test "echo '$HOME'\"$USER\""     # Vars with mixed quotes
run_test 'echo "test"test"test"'     # Alternating quotes/unquoted
run_test 'echo "te\"st"'             # Escaped quotes
run_test "echo 'te\'st'"             # Escaped single quotes
run_test 'echo "hello\nworld"'       # Escaped newline

# Variable expansion edge cases
run_test 'echo $'                    # Just dollar sign
run_test 'echo "$"'                  # Quoted dollar
run_test 'echo "$ "'                 # Dollar and space
run_test 'echo $?123'                # Exit code with trailing nums
run_test 'echo $??'                  # Multiple question marks
run_test 'echo $1234'                # Numbered var
run_test 'echo "$1234"'              # Quoted numbered var

# Mixed variable and quote tests  
run_test 'echo $HOME"$USER"'
run_test 'echo "$HOME"$USER'
run_test 'echo $HOME"$USER"text'
run_test 'echo text"$HOME"$USER'
run_test 'echo $HOME$USER"text"'

# Remove temp file
rm -f testfile
