" All system-wide defaults are set in $VIMRUNTIME/debian.vim and sourced by
" the call to :runtime you can find below.  If you wish to change any of those
" settings, you should do it in this file (/etc/vim/vimrc), since debian.vim
" will be overwritten everytime an upgrade of the vim packages is performed.
" It is recommended to make changes after sourcing debian.vim since it alters
" the value of the "compatible" option.

runtime! debian.vim

" Vim will load $VIMRUNTIME/defaults.vim if the user does not have a vimrc.
" This happens after /etc/vim/vimrc(.local) are loaded, so it will override
" any settings in these files.
" If you don"t want that to happen, uncomment the below line to prevent
" defaults.vim from being loaded.
" let g:skip_defaults_vim = 1

" Uncomment the next line to make Vim more Vi-compatible
" NOTE: debian.vim sets "nocompatible".  Setting "compatible" changes numerous
" options, so any other options should be set AFTER setting "compatible".
"set compatible

" Vim5 and later versions support syntax highlighting. Uncommenting the next
" line enables syntax highlighting by default.
if has("syntax")
  syntax on
endif

" If using a dark background within the editing area and syntax highlighting
" turn on this option as well
"set background=dark

" Uncomment the following to have Vim jump to the last position when
" reopening a file
"au BufReadPost * if line(""\"") > 1 && line(""\"") <= line("$") | exe "normal! g"\"" | endif

" Uncomment the following to have Vim load indentation rules and plugins
" according to the detected filetype.
"filetype plugin indent on

" The following are commented out as they cause vim to behave a lot
" differently from regular Vi. They are highly recommended though.
"set showcmd		" Show (partial) command in status line.
"set showmatch		" Show matching brackets.
"set ignorecase		" Do case insensitive matching
"set smartcase		" Do smart case matching
"set incsearch		" Incremental search
"set autowrite		" Automatically save before commands like :next and :make
"set hidden		" Hide buffers when they are abandoned
"set mouse=a		" Enable mouse usage (all modes)

" Source a global configuration file if available
if filereadable("/etc/vim/vimrc.local")
  source /etc/vim/vimrc.local
endif

"http://www.ruanyifeng.com/blog/2018/09/vimrc.html"
"vim function details on this address"
"also more detail in commit"

set number
"显示行码"

set nocompatible
"不与 Vi 兼容（采用 Vim 自己的操作命令)"

syntax on
"语法高亮"

set showmode
"底部显示，当前处于命令模式还是插入模式"

set showcmd
"当前键入的指令"

set mouse=a
"支持使用鼠标"

set encoding=utf-8
"使用 utf-8 编码"

set t_co=256
"启用256色"

filetype plugin indent on
"开启文件类型检查，并且载入与该类型对应的缩进规则。"
"比如，如果编辑的是.py文件，"
"Vim 就是会找 Python 的缩进规则~/.vim/indent/python.vim"

set autoindent
"下一行的缩进会自动跟上一行的缩进保持一致"

set tabstop=4
"TAB四个空格"

set cursorline
"光标所在的当前行高亮"

set cursorcolumn
"光标所在的当前列高亮"

set textwidth=80
"行宽80字"

set wrap
"自动折行"
"nowrap 关闭自动折行"

set linebreak
"不会在单词内部折行"

set showmatch
"光标遇到括号,自动高亮对应的另一个括号"

set hlsearch
"搜索时，高亮显示匹配结果"

set incsearch
"输入搜索模式时，每输入一个字符，就自动跳到第一个匹配的结果。"

set ignorecase
"搜索时忽略大小写"

set spell spelllang=en_us
"打开英语单词的拼写检查"

set history=1000
"记住多少次历史操作"

set autoread
"打开文件监视。"

set paste
"可以粘贴"

set listchars=tab:>-,trail:-
"空额替换成--"


"******************https://www.cnblogs.com/cjy15639731813/p/5886158.html"

let python_highlight_all=1
syntax on
"让你的代码变得更漂亮"

"Plugin 'scrooloose/nerdtree'"
"如果你想要一个不错的文件树形结构，那么NERDTree是不二之选"


"********************https://zhuanlan.zhihu.com/p/30022074"

"自动执行：按一下F5，自动执行代码"
map <F5> :call CompileRunGcc()<CR>
func! CompileRunGcc()
        exec "w"
        if &filetype == "c"
                exec "!g++ % -o %<"
                exec "!time ./%<"
        elseif &filetype == "cpp"
                exec "!g++ % -o %<"
                exec "!time ./%<"
        elseif &filetype == "java"
                exec "!javac %"
                exec "!time java %<"
        elseif &filetype == "sh"
                :!time bash %
        elseif &filetype == "python"
                exec "!clear"
                exec "!time python3 %"
        elseif &filetype == "html"
                exec "!firefox % &"
        elseif &filetype == "go"
                " exec "!go build %<"
                exec "!time go run %"
        elseif &filetype == "mkd"
                exec "!~/.vim/markdown.pl % > %.html &"
                exec "!firefox %.html &"
        endif
endfunc


