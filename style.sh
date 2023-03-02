#!/usr/bin/env bash

echo "Running auto checkstyle! 🔫"

# please ensure you have clang-format installed first
# sudo apt-get install clang-format

show_spinner()
{
  local -r pid="${1}"
  local -r delay='0.75'
  local spinstr='\|/-'
  local temp
  while ps a | awk '{print $1}' | grep -q "${pid}"; do
    temp="${spinstr#?}"
    printf " [%c]  " "${spinstr}"
    spinstr=${temp}${spinstr%"${temp}"}
    sleep "${delay}"
    printf "\b\b\b\b\b\b"
  done
  printf "    \b\b\b\b"
}

# run automatic checkstyle
(find . -regex '.*\.\(cpp\|h\)' -exec clang-format -style=file -i {} \;) & show_spinner "$!"

echo "Done! Thank you for not contributing to our code debt!"
