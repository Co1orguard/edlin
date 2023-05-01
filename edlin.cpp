/*
Author:     Nate Dunlap
Date:       4/11/2023
Class:      CSC-2770
Purpose:    Emulate the functionality of edlin program. These functions will 
            be "l" to list the file, "r" to read a file, "s" to save a file, 
            "e" to edit (replace) a line of the file, and "!" to run a filter 
            on a single line of the file.
*/

#include <iostream> 
#include <vector>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>


void filter(std::vector<std::string>& lines, std::vector<std::string>& user_cmd){
        
	int lineNum = stoi(user_cmd[1]);
	std::string ucmd;
	int from_parent[2];
	int to_parent[2];
	
    //iterate through user_cmd and format ucmd
	for(long unsigned int i=2; i<=user_cmd.size()-1;i++){
        	ucmd += user_cmd[i];
                if(i != user_cmd.size())
                	ucmd += " ";
       	}
	
    //create from_parent and to_parent pipes and check for errors
	if(pipe(from_parent) == -1){
		std::cerr << "from_parent could not be created" << std::endl;
	}
	if(pipe(to_parent) == -1){
		std::cerr << "to_parent could not be created" << std::endl;
	}
    
    //Call fork and determine whether execution is within the child or parent
    
    //within child process
	if(fork() == 0){
		
        
        //call dup2 on from_parent and to_parent
		dup2(from_parent[0], 0);
		dup2(to_parent[1], 1);

        //close appropriate ends of the pipe
		close(from_parent[1]);
		close(to_parent[0]);
        
        //format args 
		char* const environment[] = {NULL};
		char* args[4];
        	args[0] = (char*)"/bin/sh";
        	args[1] = (char*)"-c";
        	args[2] = (char*)ucmd.c_str();
        	args[3] = nullptr;


        // call exeve and check for errors
		if(execve(args[0], args, environment) == -1){
			std::cerr << "Execve failed to execute" << std::endl;
			exit(1);
		}
		
		
		
	}	
    
    //within parent process
	else {
        
        //close appropriate ends of the pipe 
		close(from_parent[0]);
		close(to_parent[1]); 

        //write lines[linenum] to the write end of the from_parent pipe 
		FILE* write_stream;
		write_stream = fdopen(from_parent[1], "w");
		fprintf(write_stream,  lines[lineNum].c_str());
		fclose(write_stream);
		
        
        
        // read from read end of the to_parent pipe and check for errors
		FILE* read_stream;
		read_stream = fdopen(to_parent[0], "r");
		
		char* ln = nullptr;
		size_t length = 0;
		if(getline(&ln, &length, read_stream) == -1){
			std::cerr << "Could not read from read_stream" << std::endl;
		}
		else{
			lines[lineNum] = ln;
			fclose(read_stream);
		
		}
        //close and free up all used space and pipes
		close(to_parent[0]);
		free(ln);
		wait(nullptr);	
	}
}

int main() {
    // print program title and prompt
	std::cout << "Line editor" << std::endl;
	std::cout << "edlin>";
	
    
    //declare variables for storing lines and commands
	std::string input;
	std::string operand;
	std::vector<std::string> command;
	std::vector<std::string> lines;
	
    
    //while input is not Ctrl+D 
	while(!std::cin.eof()){
        
        //get input into a stringstream object
		std::getline(std::cin, input);
        std::stringstream ss(input);	
		
        
        //iterate through ss object and populate the "command" vector
		while(getline(ss, operand, ' ')){
			command.push_back(operand);
		}
        
        //if theere is no input, print "Unknown Command" and continue prompting with edlin 
 		if(command.size() <= 0){
			std::cout << "Unknown Command" << std::endl;
			std::cout << "edlin>";
			continue;
		}
        
        // If q then quit
		if(command[0] == "q"){
			std::cout << "Exiting the editor" << std::endl;
			return 0;
		}
        
        // if not q then
		else{	
        
            // if l then iterate through lines and print all stored data
			if(command[0] == "l"){
				for(const auto& i : lines){
					std::cout << i << std::endl;
				}	
			}
			
            //if r then read from file
			else if(command[0] == "r"){
                
                //create filestream operators 
				std::string filename = command[1];
				std::ifstream targetFile(filename);
				std::string line;
                
                //if file was successfully opened then push all lines from file to our vector
				if(targetFile.is_open()){
					lines.clear();
					while(targetFile >> line){
						lines.push_back(line);
					}
				}
                
                //close file
				targetFile.close();
			}	
			else if(command[0] == "s"){
                
                //create filestrem operators
				std::string filename = command[1];
				std::ofstream targetFile(filename);
				std::string line;
                //if file was successfully opened then push all lines from vector to file
				if(targetFile.is_open()){
					for(const auto& i : lines){
	                    targetFile << i << std::endl;
        	        }

				}
                
                //close file
				targetFile.close();
			
			}
			
            
            //if e then edit file on specified line
			else if(command[0] == "e"){
				long unsigned int lineNum;
				lineNum = stoi(command[1]);
				std::string sentence;	
                
                //format string to replace
				for(long unsigned int i=2; i<=command.size()-1;i++){
                    sentence += command[i];
                    if(i != command.size())
                            sentence += " ";
                }

				//if the number supplied is more than the current vector size, append to the n+1
				if (lineNum >= lines.size()){
					lines.push_back(sentence);
				}
                // otherwise replace 
				else {
				lines[lineNum] = sentence;
				}
			}
			
            
            // if "!" then run filter
			else if(command[0] == "!"){
				

				filter(lines, command);
			}
			else{
			std::cout << "Invalid Input" << std::endl;
			}


			command.clear();
			ss.clear();	
			//read next command 
			std::cout << "edlin>";
			//getline(cin, input);
        		//std::stringstream ss(input);	
		}
	}

return 0;
}
