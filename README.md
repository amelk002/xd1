# assignment2

Or and And are the main seporaters , Or checks if the first commend executed successfully if no it executes the second part . 
And checks if the first part executed successfully than it executes the second one. Semicolon doesn't need one because it unconditionally executes the commend.

CMD uses execvp  to run the commends and keeps truck of their succession. 

Separator looks for the && , || , ; to separate commends before running them. It also orders commend in () showen in slideshow example, and some of the oder built-in command.   


Issue: Sometimes it program doesn't exit after the first exit, it take a second one to exit 
