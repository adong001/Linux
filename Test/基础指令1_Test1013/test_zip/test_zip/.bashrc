# .bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# Uncomment the following line if you don't like systemctl's auto-paging feature:
# export SYSTEMD_PAGER=

# User specific aliases and functions
alias vim='/home/test/.VimForCpp/nvim'
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/.VimForCpp/vim/bundle/YCM.so/el7.x86_64
