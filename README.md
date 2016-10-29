<h1>RSHELL</hi>
===============

<h2>Introduction</h2>
-------------------
Rshell is an interactive program that mimics the functionality of a 
bash shell with linux commands. Some commands include ls, mkdir, echo, clear, rmdir
rm, chmod and running bash scripts.

<h2>Functionality</h2>
--------------------

This program takes input from the user in the main function. 
We also used the getlogin() and gethostname() functions to 
extract the user information for display. From there,
we pass the input into a string and we then check for the "#" symbol, which
represents a comment in this program. Anything after a comment is disregarded.
After checking for a comment, we create a new string comprising of everything that
came before the comment, and we then pass this into our parse function.

Our parse function works in the following manner:<\n>
    <ol><li>removing Comments</li>
    <li>checking for presence && or || </li>
    <li>implementing commands</li></ol>

<h2>Bugfixes</h2>
<ul>
<li>ADDED chain calling functionality '|| && and ; '</li>
<li>Fixed the Bugs with valid command calls but invalid parameters</li></ul>


<h2>Bugs</h2>

<ul>
<li><bold>[FIXED]</bold>Commands that create files and folders(mkdir) return successful run, even if parameters provided are wrong for ex. mkdir bob, if bob exists, 
returns an error, but still reports a successful run


<li>Quotation marks are not supported by the echo command</li>


<li>ls cannot access directory info on the first run on hammer-- only the first time though. the function works properly when called the second time. This behaviour is observed only on hammer. It works perfectly on cloud9, Mac OSX and Ubuntu.</li>


<li>Need to fix boost tokenizer char separator for '&&' or '||' to support presence of single '&' and '|'</li>


<li>getlogin() doesn't work on cloud9. works on MACOSX hammer and Ubuntu.</li>
</ul>



