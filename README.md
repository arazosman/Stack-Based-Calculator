# Stack-Based-Calculator
YTU - Data Structures & Algorithms 2018/2 Semester Project \
A stack-based calculator which written in C programming language.
<br/><br/>


## Executing
In Unix, open up your terminal screen and go to the directory of the source files via *cd* command:

```
cd /home/USERNAME/Stack-Based-Calculator
```

Compile the program with GCC (Make sure that GCC has installed on your computer.):
```
gcc calculator.c -o Calculator
```

Run the program with its name: 
```
./Calculator
```

In Windows, to run this program you can use an IDE like CodeBlocks, Eclipse or Visual Studio.
<br/><br/>

## Usage

Let's take a look at the program menu:

```

        ########################################
        ############## CALCULATOR ##############
        ########################################


 1. Add Equation from the Terminal
 2. Add Equations from the File
 3. Print All Variables
 4. Exit

 Your choice:

```

You can give the equations to the program either on your terminal screen or with an input file. 

### Getting Equations from the Terminal

To give an equation to the program, you should follow a few simple formatting syntaxes. Assume that all the variables, numbers and arithmetic operators are words. **All words must be seperated with a space character.** For all equations, first word must be the **variable name**, which must be any of English lowercase characters, and the second one must be **equal sign**. Here is an example:

```
a = 1923 * 2018 - ( 20 + 66 )
```

Initially, all the variables in range of *a* to *z* are 0. So if you use an variable in the right-hand side of an equation without define it earlier, it acts as 0.

### Getting Equations from a File

To give equations to the program from a file, enter the input file name on the program. Make sure that the input file is in the same directory of the source file *calculator.c*. The formatting syntax of the input file is the same as above. **Also you need to add an semicolon to end of each line.** An example input file:

```
a = 3 ;
b = 2 ;
c = a + b * 4 ;
b = c + b * 2 ;
d = a * ( b - c ) / 2 ;
```
<br/>

## An Example

Let's give the equations to the program from an input file, which is below:

```
a = 3 ;
b = 2 ;
c = a + ( 1 + b * 4 ) ;
```

The output will be like this:

```
-> Getting the Postfix Expression for a = 3 :

~ Stack:
~ Postfix: 3

-> Solving the Postfix Expression 3 :

~ Stack: 3

-> Update #a: 3

#################################################

-> Getting the Postfix Expression for b = 2 :

~ Stack:
~ Postfix: 2

-> Solving the Postfix Expression 2 :

~ Stack: 2

-> Update #b: 2

#################################################

-> Getting the Postfix Expression for c = a + ( 1 + b * 4 ) :

~ Stack:
~ Postfix: a
~ Stack: +
~ Postfix: a
~ Stack: + (
~ Postfix: a
~ Stack: + (
~ Postfix: a 1
~ Stack: + ( +
~ Postfix: a 1
~ Stack: + ( +
~ Postfix: a 1 b
~ Stack: + ( + *
~ Postfix: a 1 b
~ Stack: + ( + *
~ Postfix: a 1 b 4
~ Stack: +
~ Postfix: a 1 b 4 * +
~ Stack:
~ Postfix: a 1 b 4 * + +

-> Solving the Postfix Expression a 1 b 4 * + + :

~ Stack: 3
~ Stack: 3 1
~ Stack: 3 1 2
~ Stack: 3 1 2 4
~ Stack: 3 1 8
~ Stack: 3 9
~ Stack: 12

-> Update #c: 12
```
