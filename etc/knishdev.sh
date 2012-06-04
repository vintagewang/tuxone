# sys env
export PS1='[$LOGNAME]redhat-x64:$PWD>'
export PATH=.:$PATH:/sbin

# knish
export KNISH_HOME=$HOME/wolf/github/tuxone
export PATH=$KNISH_HOME/code/bin:$KNISH_HOME/code/ext/lwpr/bin:$KNISH_HOME/sbin:$PATH
export LD_LIBRARY_PATH=$KNISH_HOME/code/lib:$KNISH_HOME/code/ext/lwpr/lib:$LD_LIBRARY_PATH
. $KNISH_HOME/code/tux.env

# alias
alias cdh='cd $HOME/htc'
alias cdk='cd $KNISH_HOME'
alias cdkl='cd $KNISH_HOME/code/ext/lwpr'
alias cdkls='cd $KNISH_HOME/code/ext/lwpr/src'
alias cdklp='cd $KNISH_HOME/code/ext/lwpr/project'
alias cdklb='cd $KNISH_HOME/code/ext/lwpr/bin'
alias cdklt='cd $KNISH_HOME/code/ext/lwpr/tools'
alias cdks='cd $KNISH_HOME/code/src'
alias cdksg='cd $KNISH_HOME/code/src/txgate'
alias cdkp='cd $KNISH_HOME/code/project'
alias cdkb='cd $KNISH_HOME/code/bin'
alias cdka='cd $KNISH_HOME/code/samples/atmi/simpapp'
alias cdke='cd $KNISH_HOME/code/samples/atmi/echo'
alias cdkv='cd $KNISH_HOME/code/samples/atmi/view32'
alias cdkf='cd $KNISH_HOME/code/samples/atmi/fml32'

# other command
ulimit -c unlimited
