#include <unistd.h>
#include <cstring>
#include <utility>
#include <algorithm>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include "Commands.h"
#include <vector>
//#include <stl_algo.h>

using namespace std;
#define WHITESPACE ' '


#if 0
#define FUNC_ENTRY()  \
  cout << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
  cout << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif

string _ltrim(const std::string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

string _rtrim(const std::string& s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string _trim(const std::string& s)
{
    return _rtrim(_ltrim(s));
}

int _parseCommandLine(const char* cmd_line, char** args) {
    FUNC_ENTRY()
    int i = 0;
    std::istringstream iss(_trim(string(cmd_line)).c_str());
    for(std::string s; iss >> s; ) {
        args[i] = (char*)malloc(s.length()+1);
        memset(args[i], 0, s.length()+1);
        strcpy(args[i], s.c_str());
        args[++i] = NULL;
    }
    return i;

    FUNC_EXIT()
}

bool _isBackgroundComamnd(const char* cmd_line) {
    const string str(cmd_line);
    return str[str.find_last_not_of(WHITESPACE)] == '&';
}

void _removeBackgroundSign(char* cmd_line) {
    const string str(cmd_line);
    // find last character other than spaces
    unsigned int idx = str.find_last_not_of(WHITESPACE);
    // if all characters are spaces then return
    if (idx == string::npos) {
        return;
    }
    // if the command line does not end with & then return
    if (cmd_line[idx] != '&') {
        return;
    }
    // replace the & (background sign) with space and then remove all tailing spaces.
    cmd_line[idx] = ' ';
    // truncate the command line string up to the last non-space character
    cmd_line[str.find_last_not_of(WHITESPACE, idx) + 1] = 0;
}


/**  helper functions **/


static bool IsNumber(const string& str){
    if(str[0]=='-'){
        for(unsigned int i=1;i<str.length();i++){
            if(std::isdigit(str[i])==0){
                return false;
            }
        }
        return true;
    }
    for(char const &c:str){
        if(std::isdigit(c)==0) return false;
    }
    return true;
}








/**  SmallShell class methods **/
SmallShell::SmallShell() :mCurrentFg(nullptr),mPrevPwd(nullptr),mUseCustomPrompt(false)
        ,mpJobsList(),newlist(),mPid(getpid()),mLastPrompt(
                nullptr){
    this->mpJobsList=new JobsList();
}





/**  Command class methods **/

Command::Command(const char *cmdLine):mNewCmd(cmdLine){
    if(_isBackgroundComamnd(cmdLine)) this->mIsCommandBackground= true;
//    mpJobsList->RemoveFinishedJobs();
    string first_word = _trim(string(cmdLine));
//    bool is_background = _isBackgroundComamnd(cmdLine);
    if (first_word[first_word.length() - 1] == '&') {
        first_word = first_word.substr(0, first_word.length() - 1);
    }
    string commandS = _trim(string(cmdLine));
//    char *args[ARGS_ARRAY_LENGTH];
    first_word = first_word.substr(0, commandS.find_first_of(" \n"));
    if(first_word=="cd" || first_word=="pwd" || first_word=="chprompt"|| first_word=="showpid") this->IsCommandBuiltIn= true;
    if(first_word=="jobs" || first_word=="kill" || first_word=="fg"|| first_word=="bg") this->IsCommandBuiltIn= true;
    if(first_word=="quit") this->IsCommandBuiltIn= true;

//    this->mNewCmd=(char*)malloc(sizeof (char)* strlen(cmdLine) +1);
//    this->mNewCmd(cmdLine);
//    strcpy(mNewCmd, cmdLine);
    if(_isBackgroundComamnd(cmdLine)) mIsCommandBackground = true;
//    _removeBackgroundSign(mNewCmd);
}




/**  ExternalCommand class methods **/



ExternalCommand::ExternalCommand(const char *cmd_line) : Command(cmd_line){}

void ExternalCommand::execute(char **args, int num) {
//    string t=mNewCmd;
//    _removeBackgroundSign(mNewCmd);
    pid_t pid=fork();
    std::string t=this->mNewCmd;
//    std::cout<< t << std::endl;
//    t.erase(std::remove)
    t.erase(std::remove(t.begin(),t.end(),'&'),t.end());
    setpgrp();
    char*  argv[4]={const_cast<char*>("/bin/bash"),const_cast<char*>("-c"),const_cast<char*>(t.c_str()), nullptr};

//    char* binBash= const_cast<char*>("/bin/bash");
//    char* minC=const_cast<char*>("c");
//
//    char*  argv[4]={binBash,minC,const_cast<char*>(t.c_str()), nullptr};

//    string bash={"/bin/bash\0"};
//    char*  argv[4]={"/bin/bash","-c",const_cast<char*>(t.c_str()), nullptr};

    if(pid == -1) //fail
    {
        perror("smash failed: fork failed");
        return;
    }
    if(pid==0) // child process
    {
        execv("/bin/bash",argv);
        /** we don't need an if statement because in case execv fails it returns here, and
         * if it does not fail it doesnt return to this line (that's how execv works) **/
        perror("smash error: execvp failed");
        exit(0);
    }
    bool flag= false;
    if (pid != 0) //father process
    {
        SmallShell& smash = SmallShell::getInstance();

        JobsList::JobEntry to_enter_job =JobsList::JobEntry();
        to_enter_job.mJobStatus=background;
        to_enter_job.mPid=pid;
        to_enter_job.mJobId=smash.mpJobsList->GetMaxId()+1;
        to_enter_job.command=this->mNewCmd;
        to_enter_job.isBackground= true;
        to_enter_job.mEntryTime=time(nullptr);
//        smash.mpJobsList->list.push_back(to_enter_job);

        if(!this->mIsCommandBackground){
            SmallShell& smash = SmallShell::getInstance();
            smash.mCurrentFg= &to_enter_job;
            if(waitpid(pid, nullptr,WUNTRACED) == -1){
                perror("smash error: waitpid failed");
                return;
            }
            smash.mCurrentFg= nullptr;
//            exit(0);
//            flag= true;
            return;
        }


//        SmallShell& smash = SmallShell::getInstance();
        /**  here it is a background command that needs to be executed in the background
         * so we need to add a new job**/
//         JobsList::JobEntry to_enter_job =JobsList::JobEntry();
//         to_enter_job.mJobStatus=background;
//         to_enter_job.mPid=pid;
//         to_enter_job.mJobId=smash.mpJobsList->GetMaxId()+1;
//         to_enter_job.command=this->mNewCmd;
//         to_enter_job.isBackground= true;
//         to_enter_job.mEntryTime=time(nullptr);


        if(!flag){
            for (JobsList::JobEntry &job: smash.mpJobsList->list) {
                if (job.mJobStatus == empty) {
                    job.mJobStatus = background;
                    job.mPid = pid;
                    job.mJobId = smash.mpJobsList->GetMaxId() + 1;
                    job.command = mNewCmd;
                    job.isBackground = true;
                    job.mEntryTime = time(nullptr);
                    return;
                }

            }
            smash.mpJobsList->list.push_back(to_enter_job);
        }
        return;
//         int max_id= smash.mpJobsList->GetMaxId();
//         JobsList::JobEntry x
    }


//    t.erase(std::remove(t.begin(),t.end(),'&'),t.end());
}





/**  BuiltInCommand class methods **/

BuiltInCommand::BuiltInCommand(const char *cmd_line) : Command(cmd_line){}

void BuiltInCommand::execute(char **args, int numOfArgs) {}








/**  quit command, "quit", QuitCommand class methods **/

QuitCommand::QuitCommand(const char *cmd_line) : BuiltInCommand(cmd_line){};

void QuitCommand::execute(char **args, int num) {
    auto& shell=SmallShell::getInstance();
    shell.mpJobsList->RemoveFinishedJobs();
    bool flag= false;
    for (int i = 0; i < num; ++i) {
        if(strcmp(args[i],"kill")==0)
            flag= true;
    }
    if(flag){
        std::cout<<"smash: sending SIGKILL signal to "<<shell.mpJobsList->GetNumOfJobs()<<" jobs:"<<std::endl;
        shell.mpJobsList->PrintJobsListForQuit();
        shell.mpJobsList->KillAllJobs();
    }
//    shell.mpJobsList->KillAllJobs();
    exit(0);
}




/**  send to background command, "bg", BackgroundCommand class methods **/


BackgroundCommand::BackgroundCommand(const char *cmd_line) : BuiltInCommand(cmd_line){}

void BackgroundCommand::execute(char **args, int num) {
    if(num >2 ){
        std::cerr<<"smash error: bg: invalid arguments"<<std::endl;
        return;
    }
    if(num==2){
        if(args[1][0] == '-'){
            std::cerr<<"smash error: bg: job-id "<<args[1]<<" does not exist"<<std::endl;
            return;
        }

        if(!IsNumber(args[1])){
            std::cerr << "smash error: bg: invalid arguments" << std::endl;
            return;
        }
        auto& shell=SmallShell::getInstance();
        JobsList::JobEntry* it=shell.mpJobsList->GetJobById(std::stoi(args[1]));
        if(it== nullptr){
            std::cerr<<"smash error: bg: job-id "<<args[1]<<" does not exist"<<std::endl;
            return;
        }
        if(it->mJobStatus==background){
            std::cerr<<"smash error: bg: job-id "<<args[1]<<" is already running in the background"<<std::endl;
            return;
        }
        if(kill(it->mPid, SIGCONT)== -1) {
            cerr << "smash error: kill failed" << std::endl;
        }
        std::cout<<it->command;
        std::cout<<" : "<<it->mPid<<std::endl;
        it->mJobStatus=background;
        return;
    } else if(num==1){
        auto& shell=SmallShell::getInstance();
        if(!shell.mpJobsList->IsThereAnyStopped()){
            std::cerr<<"smash error: bg: there is no stopped jobs to resume"<<std::endl;
            return;
        }
        JobsList::JobEntry* it=shell.mpJobsList->GetLastStoppedJob();
//        if(it== nullptr){
//
//        }
        if(kill(it->mPid, SIGCONT)== -1) {
            cerr << "smash error: kill failed" << std::endl;
        }
        std::cout<<it->command;
        std::cout<<" : "<<it->mPid<<std::endl;
        it->mJobStatus=background;
    }
}




/**  bring to foreground command, "fg", ForegroundCommand class methods **/


ForegroundCommand::ForegroundCommand(const char *cmd_line) : BuiltInCommand(cmd_line){}

void ForegroundCommand::execute(char **args, int num) {
    if(num >2 ){
        std::cerr<<"smash error: fg: invalid arguments"<<std::endl;
        return;
    }
    if(num==2){
        if(args[1][0]=='-'){
            std::cerr<<"smash error: fg: job-id "<< std::stoi(args[1])<<" does not exist"<<std::endl;
            return;

        }
//        int helper=std::atoi(args[1]);
        if( !IsNumber(args[1])) {
            std::cerr << "smash error: fg: invalid arguments" << std::endl;
            return;
        }

        auto& shell= SmallShell::getInstance();
        if(shell.mpJobsList->IsEmpty()){
            std::cerr<<"smash error: fg: jobs list is empty"<<std::endl;
            return;
        }
        JobsList::JobEntry* it=shell.mpJobsList->GetJobById(std::stoi(args[1]));
        if(it== nullptr){
            std::cerr<<"smash error: fg: job-id "<< std::stoi(args[1])<<" does not exist"<<std::endl;
            return;
        } else{
            if(kill(it->mPid, SIGCONT)== -1)
            {
                cerr<<"smash error: kill failed"<<std::endl;
                return;
            }
            std::cout<<it->command;
//            if(it->isBackground) std::cout<<" &";
            std::cout<<" : "<<it->mPid<<std::endl;
            int status;
            it->mJobStatus=foreground;

//            shell.mpJobsList->list.erase(it);
            shell.mCurrentFg=it;
            shell.mCurrentFg->mJobStatus=foreground;
            waitpid(it->mPid, &status, WUNTRACED);
        }
    } else if(num==1)
    {
        auto& shell = SmallShell::getInstance();
        if(shell.mpJobsList->IsEmpty()){
            std::cerr<<"smash error: fg: jobs list is empty"<<std::endl;
            return;
        }
        JobsList::JobEntry* it=shell.mpJobsList->GetJobById(shell.mpJobsList->GetMaxId());
        if(it== nullptr){
            std::cerr<<"smash error: fg: job-id "<< std::stoi(args[1])<<" does not exist"<<std::endl;
            return;
        }else{
            if(kill(it->mPid, SIGCONT)== -1)
            {
                cerr<<"smash error: kill failed"<<std::endl;
                return;
            }
            std::cout<<it->command;
//            if(it->isBackground) std::cout<<" &";
            std::cout<<" : "<<it->mPid<<std::endl;
            int status;
            it->mJobStatus=foreground;
//            shell.mpJobsList->list.erase(it);
            shell.mCurrentFg=it;
            shell.mCurrentFg->mJobStatus=foreground;
            waitpid(it->mPid, &status, WUNTRACED);
        }
    }

}







/**  kill command, "kill", KillCommand class methods **/


KillCommand::KillCommand(const char *cmd_line) : BuiltInCommand(cmd_line){};

void KillCommand::execute(char **args, int num) {
    if(num!= 3) {
        std::cerr<< "smash error: kill: invalid arguments"<<std::endl;
        return;
    }

    if(!args[1] || !IsNumber(args[2]) || !IsNumber(args[1])) {

        std::cerr<< "smash error: kill: invalid arguments"<<std::endl;
        return;
    }
    if(args[1][0] != '-') {

        std::cerr<< "smash error: kill: invalid arguments"<<std::endl;
        return;
    }
    if(strlen(args[1])<=1){
        std::cerr<<"smash error: kill: invalid arguments"<<std::endl;
        return;
    }

    string helper=args[1];
    helper.erase(0,1);
//    if(!IsNumber(helper)){
//        std::cerr<<"smash error: kill: invalid arguments"<<std::endl;
//        return;
//    }
    int sig= std::stoi(helper);
//    if(sig>=)

    auto& shell = SmallShell::getInstance();
    JobsList::JobEntry* it= shell.mpJobsList->GetJobById(std::stoi(args[2]));
    if(it!= nullptr)
    {

        if(kill(it->mPid, sig) == -1)
        {
            perror("smash error: kill failed");
//            std::cout<< "smash error: kill failed"<<std::endl;
            return;
        }
        if(sig == SIGSTOP || sig ==SIGTSTP){
            it->mJobStatus=stopped;
        } else if( sig == SIGCONT){
//            {
//                if(it->isBackground){
//                    it->mJobStatus=background;
//                }
//            }
            it->mJobStatus=background;
        } else if(sig == SIGKILL)
        {
            it->mJobStatus=finished;
        }

    }else {
        std::cerr << "smash error: kill: job-id " << std::stoi(args[2]) << " does not exist" << std::endl;
        return;
    }
    std::cout<<"signal number "<< sig<<" was sent to pid "<< it->mPid<<std::endl;

}



/**  print jobs command, "jobs", JobsCommand class methods **/

JobsCommand::JobsCommand(const char *cmd_line) : BuiltInCommand(cmd_line){};

void JobsCommand::execute(char **args, int num) {
    auto& shell = SmallShell::getInstance();
//    shell.mpJobsList->RemoveFinishedJobs();
    shell.mpJobsList->PrintJobsList();
}


/**  JobsList class methods **/

JobsList::JobEntry *JobsList::GetJobByPid(int Pid) {
    for (JobEntry& job: list) {
        if(Pid==job.mPid) return &job;
    }
    return nullptr;
}

int JobsList::GetNumOfJobs() {
    int it=0;
    for (JobEntry& job:list) {
        if(job.mPid==0 || job.mJobStatus==empty || job.mJobId==0) continue;
        it++;

    }
    return it;
}


void JobsList::PrintJobsList() {
//    this->RemoveFinishedJobs();
    time_t time_on_exec= time(nullptr);
    for (int i=0 ;i<this->GetMaxId()+10;i++) {//change to : for(JobsEntry& job: list)
        JobEntry* it= this->GetJobById(i);
        if(it== nullptr) continue;
        if(it->mJobStatus!=empty)
        {
            std::cout << "[" << it->mJobId << "] " << it->command;
//                if (it.isBackground)
//                    std::cout << "&";
            std::cout << " : " << it->mPid << " " << difftime(time_on_exec , it->mEntryTime) << " secs";
            if (it->mJobStatus == gotCtrlZ)
                std::cout << " (stopped)";
            std::cout << std::endl;
        }

    }
}

void JobsList::PrintJobsListForQuit() {
    this->RemoveFinishedJobs();
    for (int i=0; i<this->GetMaxId()+10;i++){
        JobEntry* it= GetJobById(i);
        if(it== nullptr)continue;
        if(it->mJobStatus==empty ||it->mPid==0 || it->mJobId ==0 ) continue;
        std::cout<<it->mPid<<": "<<it->command<<std::endl;
//        if(job.isBackground) std::cout<<"&";
//        std::cout<<std::endl;
    }
}

void JobsList::RemoveJobById(int id) {
    for (JobEntry& job: list) {
        if(job.mJobId==id){
            job.mJobStatus=empty;
            job.mJobId=0;
            job.mPid=0;
            job.isBackground=false;
            job.mEntryTime=0;
        }
    }
}

void JobsList::KillAllJobs() {
    this->RemoveFinishedJobs();
    for (JobEntry& job: list){
        if(job.mPid==0 || job.mJobStatus==empty || job.mJobId==0) continue;
        if(kill(job.mPid,SIGKILL)==-1){
            cerr<<"smash error: kill failed"<<std::endl;
        }
    }
}

bool JobsList::IsEmpty() {
    for (JobEntry& job: list) {
        if(job.mJobStatus==stopped || job.mJobStatus==background || job.mJobStatus==gotCtrlZ) return false;
    }
    return true;
}

bool JobsList::IsThereAnyStopped() {
    for (JobEntry& job:this->list)
    {
        if(job.mJobStatus==gotCtrlZ) return true;
    }
    return false;
}

JobsList::JobEntry *JobsList::GetJobById(int id) {
    if(id==-1 ) { return nullptr; }
    for (JobEntry &job: list) {
        if (job.mJobId == id)
            return &job;
    }
    return nullptr;
}

int JobsList::GetMaxId() {
    int res=0;
    for(JobEntry& job : this->list){
        if((job.mJobId > res) && (job.mJobStatus==stopped || job.mJobStatus==background || job.mJobStatus==gotCtrlZ) ){
            res =job.mJobId;
        }
    }
    return res;
}







JobsList::JobEntry *JobsList::GetLastJob() {
    int max_id=-1;
    for (JobEntry& job:list){
        if(job.mJobId > max_id) max_id=job.mJobId;
    }
    return GetJobById(max_id);
}

JobsList::JobEntry *JobsList::GetLastStoppedJob() {
    int max_id=-1;
    for (JobEntry& job : list){
        if((job.mJobStatus==gotCtrlZ) && job.mJobId>max_id)
            max_id=job.mJobId;
    }
    return GetJobById(max_id);
}


void JobsList::RemoveFinishedJobs() {
    for (JobEntry &job: list) {
        if(job.mPid==0 || job.mJobStatus==empty || job.mJobId==0) continue;
        if (job.mJobStatus != stopped && job.mJobStatus!=gotCtrlZ) {
            if (waitpid(job.mPid, nullptr, WNOHANG) != 0)
                job.mJobStatus = finished;
        }
    }



    for (JobEntry& job: list) {
        if(job.mJobStatus==finished){
            this->RemoveJobById(job.mJobId);
        }
    }
}




//    if(id>100 || id<0) { return nullptr; }
//    JobEntry* it=list.begin();
//    for (;  it!= list.end() ; it++) {
//        if(it->mJobId==id)
//            return it;
//    }
//    return nullptr;
//}






/**  change directory, "cd", changeDirCommand class methods **/
ChangeDirCommand::ChangeDirCommand(const char *cmd_line): BuiltInCommand(cmd_line) {}

void ChangeDirCommand::execute(char **args, int num) {
    if(args== nullptr) return;
    if(num >2 ){
        std::cerr << "smash error: cd: too many arguments" << std::endl;
        return;
    }
    if(num ==1) return;
    auto& shell=SmallShell::getInstance();
    if(args[1][0]=='-') {
        if (shell.mPrevPwd == nullptr) {
            std::cerr << "smash error: cd: OLDPWD not set" << std::endl;
            return;
        }
        else if(strlen(args[1]) !=1) {
            std::cerr << "smash error: cd: too many arguments" << std::endl;
            return;
        }
        else{
            //char* it=malloc(sizeof (char)* strlen(shell.mPrevPwd) +1);
            char* tmp=get_current_dir_name();
            if(chdir(shell.mPrevPwd)==-1){
                perror("smash error: chdir failed");
                return;
            }
            //            strcpy(it,shell.mPrevPwd);
            free(shell.mPrevPwd);
            shell.mPrevPwd=tmp;
            return;
        }
    }

    else {
        char* tmp=get_current_dir_name();

        if(chdir(args[1])==-1)
        {
            perror("smash error: chdir failed");
//            std::cout<<"fuckkkkkkk"<<std::endl;
            return;
        }
        if (shell.mPrevPwd != nullptr)
            free(shell.mPrevPwd);
        shell.mPrevPwd=tmp;
    }
}






/**  get current working directory, "pwd", getCurrDirCommand class methods **/
GetCurrDirCommand::GetCurrDirCommand(const char *cmd_line) : BuiltInCommand(cmd_line){}

void GetCurrDirCommand::execute(char **args, int num) {
    char* cwd=get_current_dir_name();
    if(cwd== nullptr)
        return;
//    string it=string(cwd);
    std::cout << cwd << std::endl;
    free(cwd);
}




/**  show pid command, "showPid", showPid class methods **/
ShowPidCommand::ShowPidCommand(const char *cmd_line): BuiltInCommand(cmd_line){};

void ShowPidCommand::execute(char **args, int num) {
    auto& shell=SmallShell::getInstance();
    std::cout << "smash pid is " << shell.mPid << std::endl;
}



/**  change prompt command, "chprompt" , chprompt class methods **/

ChPromptCommand::ChPromptCommand(const char *cmd) : BuiltInCommand(cmd){};

void ChPromptCommand::execute(char **args, int num) {
    auto& shell=SmallShell::getInstance();
    if(num==1)
    {
        if(shell.mUseCustomPrompt){
            free(shell.mLastPrompt);
        }
        shell.mUseCustomPrompt= false;
    } else
    {
        if(shell.mUseCustomPrompt)
        {
            free( shell.mLastPrompt);
        }
        shell.mUseCustomPrompt=true;
        int x= strlen(args[1]);
        shell.mLastPrompt= (char*)malloc(sizeof(char)*x +1);
        strcpy(shell.mLastPrompt,args[1]);
    }
}



/**  HeadCommand command, "Head" , HeadCommand class methods **/
HeadCommand::HeadCommand(const char *cmd_line) : BuiltInCommand(cmd_line){}

void HeadCommand::execute(char **args, int num) {
    int reps=10,file_disc,size=0;
    if(num<=1){
        std::cerr<<"smash error: head: not enough arguments" << std::endl;
        return;
    }
    if(num>=4){
        perror("smash error: head: too many arguments");
        return;
    }
    if(num==3){
        if(args[1][0] != '-'){
            perror("smash error: head: invalid argument");
            return;
        }
        if(strlen(args[1]) ==1){
            perror("smash error: head: invalid argument");
            return;
        }
        string helper= args[1];
        helper.erase(0,1);
        reps= stoi(helper);
        file_disc= open(args[2], O_RDONLY);
        if(file_disc ==-1) {
            perror("smash error: open failed");
            return;
        }
    }
    if(num==2){
        file_disc= open(args[1], O_RDONLY);
        if(file_disc==-1){
            perror("smash error: open failed");
            return;
        }
    }


    char our_char='z';
    size=read(file_disc, &our_char, 1);
    if(size==-1){
        perror("smash error: read failed");
        return;
    }
    if(our_char == EOF) return;
    while (size>0 && reps >0){
        if(our_char == EOF)
            return;
        if(our_char == '\n'){
            reps--;
        }
        if(write(1, &our_char,1) == -1){
            perror("smash error: write failed");
            return;
        }
        size=read(file_disc,&our_char,1);
        if(size==-1){
            perror("smash error: read failed");
            return;
        }
    }


}
/**  Redirection command, "> or >> " , RedirectionCommand class methods **/

RedirectionCommand::RedirectionCommand(const char *cmd_line, bool append): Command(cmd_line),append(append) {}

void RedirectionCommand::execute(char **args, int num) {

    // ~~~~~~~~~~~~~~~~~~~~~Command Trim~~~~~~~~~~~~~~~~~~~
    string cmd;
    unsigned long index;
    if(this->append){
        cmd=mNewCmd.substr(0,mNewCmd.find(">"));
        index= mNewCmd.find(">")+2;
    } else {
        cmd=mNewCmd.substr(0,mNewCmd.find(">"));
        index= mNewCmd.find(">")+1;
    }

    string outfile= mNewCmd.substr(index,mNewCmd.size());
    cmd= _trim(cmd);
    outfile= _trim(outfile);
    if(outfile.find("&") != string::npos) {
        outfile = outfile.erase(outfile.find_last_of("&"));
    }
    outfile= _trim(outfile);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    int fd_new=0;
    if(this->append){
        fd_new= open(outfile.c_str(), O_WRONLY | O_CREAT | O_APPEND , 0666);
    }
    else{
        fd_new= open(outfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC , 0666);
    }
    if(fd_new == -1){
        perror("smash error: open failed");
        return;
    }

    int fd=dup(1);

    if(fd==-1){ // save standard stdout
        perror("smash error: dup failed");
        return;
    }
    if(close(1) == -1){
        perror("smash error: close failed");
        return;
    }
    if(dup2(fd_new,1) == -1){
        perror("smash error: dup failed");
        return;
    }

    SmallShell& shell= SmallShell::getInstance();
    Command* command_ptr = shell.CreateCommand(cmd.c_str());
    char* argsNew[COMMAND_ARGS_MAX_LENGTH+3];
//    auto x= const_cast<char*>(this->mNewCmd);
    int numNEw= _parseCommandLine(cmd.c_str(),argsNew);
    command_ptr->execute(argsNew,numNEw);
    for (int i = 0; i <  numNEw; ++i) {
        if(argsNew[i]) (free(argsNew[i]));
    }
    if(close(1) == -1){
        perror("smash failed: close failed");
        return;
    }
    if(dup(fd) == -1){// =dup2(fd,1)
        perror("smash error: dup failed");
        return;
    }
    if(close(fd) == -1) {
        perror("smash error: close failed");
       return;
    }



}

/**  PipeCommand command, "& or |&" , PipeCommand class methods **/


PipeCommand::PipeCommand(const char *cmd_line, bool background) : Command(cmd_line),mBackgroundPipe(background){}

void PipeCommand::execute(char **args, int num) {
//    SmallShell &smash = SmallShell::getInstance();
//    smash.mpJobsList->RemoveFinishedJobs();


    int pipeFD[2];
    bool isBack=this->mBackgroundPipe;
    pipe(pipeFD);
    SmallShell &shell = SmallShell::getInstance();
    shell.mpJobsList->RemoveFinishedJobs();
    int  stdInFd=dup(0);
    int stdOutFd=dup(1);
    int stdErrFd=dup(2);

//    setpgrp();
    string barrier = this->mBackgroundPipe ? "|&" : "|";
    string left_cmd = mNewCmd.substr(0, mNewCmd.find(barrier));
    auto index = !this->mBackgroundPipe ? mNewCmd.find(barrier) + 1 : mNewCmd.find(barrier) + 2;
    string right_cmd = mNewCmd.substr(index, mNewCmd.size());
    left_cmd = _ltrim(_rtrim(left_cmd));
    right_cmd = _ltrim(_rtrim(right_cmd));


    if (right_cmd.find("&") != string::npos) {
        right_cmd = right_cmd.erase(right_cmd.find_last_of("&"));
        right_cmd = _ltrim(_rtrim(right_cmd));
    }
    SmallShell &tmp = SmallShell::getInstance();
    shared_ptr<Command>  cmd_left(tmp.CreateCommand(left_cmd.c_str()));
    shared_ptr<Command> cmd_right(tmp.CreateCommand(right_cmd.c_str()));

    int firstFork=fork();
    if (firstFork ==0) {
//        SmallShell &shell = SmallShell::getInstance();
//        shell.mpJobsList->RemoveFinishedJobs();
//        setpgrp();
//        string barrier = this->mBackgroundPipe ? "|&" : "|";
//        string left_cmd = mNewCmd.substr(0, mNewCmd.find(barrier));
//        auto index = !this->mBackgroundPipe ? mNewCmd.find(barrier) + 1 : mNewCmd.find(barrier) + 2;
//        string right_cmd = mNewCmd.substr(index, mNewCmd.size());
//        left_cmd = _ltrim(_rtrim(left_cmd));
//        right_cmd = _ltrim(_rtrim(right_cmd));
//
//
//        if (right_cmd.find("&") != string::npos) {
//            right_cmd = right_cmd.erase(right_cmd.find_last_of("&"));
//            right_cmd = _ltrim(_rtrim(right_cmd));
//        }
//        SmallShell &tmp = SmallShell::getInstance();
//        shared_ptr<Command> cmd_left(tmp.CreateCommand(left_cmd.c_str()));
//        int stdFD=0, stdfd=0;
        if (!isBack) {
//            stdFD=dup(1);
            dup2(pipeFD[1], 1);
        } else {
//            stdfd=dup(2);
            dup2(pipeFD[1], 2);
        }
//        close(pipeFD[0]);
//        dup2(pipeFD[1],1);
        close(pipeFD[0]);
        close(pipeFD[1]);
        char* argsf[COMMAND_ARGS_MAX_LENGTH+3];
        int numf= _parseCommandLine(left_cmd.c_str(),argsf);
        cmd_left->execute(argsf,numf);
        for (int i = 0; i <  numf; ++i) {
            if(argsf[i]) (free(argsf[i]));
        }
        if(!this->mBackgroundPipe){
            dup2(stdOutFd,1);
//            close(stdFD);
            exit(0);
        }
        dup2(stdErrFd,2);

        exit(0);

//        if (!this->mBackgroundPipe) {
//            auto stdFD=dup(1);
//            dup2(pipeFD[1], 1);
//        } else {
//            auto stdfd=dup(2);
//            dup2(pipeFD[1], 2);
//        }
////        close(pipeFD[1]);
//        close(pipeFD[0]);
//        cmd_left->execute();
//        exit(0);
    }
    else if(firstFork ==-1){
        perror("smash error: fork failed");
        return;
    }
    int secFork=fork();
    if (secFork == 0) {
//        SmallShell &shell = SmallShell::getInstance();
//        shell.mpJobsList->RemoveFinishedJobs();
////        dup2(pipeFD[0], 0);
////        close(pipeFD[0]);
////        close(pipeFD[1]);
//        string barrier = this->mBackgroundPipe ? "|&" : "|";
//        string left_cmd = mNewCmd.substr(0, mNewCmd.find(barrier));
//        auto index = !this->mBackgroundPipe ? mNewCmd.find(barrier) + 1 : mNewCmd.find(barrier) + 2;
//        string right_cmd = mNewCmd.substr(index, mNewCmd.size());
//        left_cmd = _ltrim(_rtrim(left_cmd));
//        right_cmd = _ltrim(_rtrim(right_cmd));
//
//        if (right_cmd.find("&") != string::npos) {
//            right_cmd = right_cmd.erase(right_cmd.find_last_of("&"));
//            right_cmd = _ltrim(_rtrim(right_cmd));
//        }
//
//        SmallShell &tmp = SmallShell::getInstance();
//        shared_ptr<Command> cmd_right(tmp.CreateCommand(right_cmd.c_str()));
//        int stdFD=0, stdfd=0;
        if (!isBack) {
//            stdFD=dup(0);
            dup2(pipeFD[0], 0);
        } else {
//            stdfd=dup(2);
            dup2(pipeFD[0], 0);
        }
//        close(pipeFD[1]);
//        dup2(pipeFD[0],0);
        close(pipeFD[0]);
        close(pipeFD[1]);
        char* argss[COMMAND_ARGS_MAX_LENGTH+3];
        int nums= _parseCommandLine(right_cmd.c_str(),argss);
        cmd_right->execute(argss,nums);
        for (int i = 0; i <  nums; ++i) {
            if(argss[i]) (free(argss[i]));
        }
//        if(!this->mBackgroundPipe){
//            dup2(stdFD,0);
////            close(stdFD);
//            exit(0);
//        }
//        dup2(stdfd,0);
        dup2(stdInFd,0);
        exit(0);
    }
    else if( secFork == -1){
        perror("smash error: fork failed");
        return;
    }


    if(close(pipeFD[0])==-1){
        perror("smash error: close failed");
    }
    if(close(pipeFD[1])==-1){
        perror("smash error: close failed");
    }
    close(stdInFd);
    close(stdOutFd);
    close(stdErrFd);
    if(waitpid(firstFork, nullptr,0)==-1){
        perror("smash error: waitpid failed");
        return;
    }
    if(waitpid(secFork, nullptr,0)==-1){
        perror("smash error: waitpid failed");
        return;
    }

}

// TODO: Add your implementation for classes in Commands.h

//SmallShell::SmallShell() {
//// TODO: add your implementation
//}

SmallShell::~SmallShell() {
// TODO: add your implementation
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command * SmallShell::CreateCommand(const char* cmd_line) {
    mpJobsList->RemoveFinishedJobs();
    string first_word = _trim(string(cmd_line));
//    bool is_background = _isBackgroundComamnd(cmd_line);
    if (first_word[first_word.length() - 1] == '&') {
        first_word = first_word.substr(0, first_word.length() - 1);
    }
    string commandS = _trim(string(cmd_line));
//    char *args[ARGS_ARRAY_LENGTH];
    first_word = first_word.substr(0, commandS.find_first_of(" \n"));
    bool pipe_bg = commandS.find("|&") != std::string::npos;
    bool pipe_fg = commandS.find("|") != std::string::npos;
    bool redirect_append = commandS.find(">>") != std::string::npos;
    bool redirect_overwrite = commandS.find(">") != std::string::npos;

    if(pipe_fg || pipe_bg){
        return new PipeCommand(cmd_line,pipe_bg);
    }
    if( redirect_append || redirect_overwrite){
        return new RedirectionCommand(cmd_line,redirect_append);
    }
    SmallShell& shell = SmallShell::getInstance();
    shell.mpJobsList->RemoveFinishedJobs();



    if (first_word == "chprompt") {
        return new ChPromptCommand(cmd_line);

    } else if (first_word == "head") {
        return new HeadCommand(cmd_line);

    } else if (first_word == "showpid") {
        return new ShowPidCommand(cmd_line);

    } else if (first_word == "pwd") {
        return new GetCurrDirCommand(cmd_line);

    } else if (first_word == "cd") {
        return new ChangeDirCommand(cmd_line);

    } else if (first_word == "jobs") {
        return new JobsCommand(cmd_line);

    } else if (first_word == "quit") {
        return new QuitCommand(cmd_line);

    } else if (first_word == "kill") {
        return new KillCommand(cmd_line);

    } else if (first_word == "fg") {
        return new ForegroundCommand(cmd_line);

    } else if (first_word == "bg") {
        return new BackgroundCommand(cmd_line);
    }
    return new ExternalCommand(cmd_line);
}



//  if(commandS.find(">>") != std::string::nops){
//      mIsAppendRedirectCmd= true;
//  } else if(commandS.find(">") != std::string::nops){
//      mIsOverwriteRedirectCmd= true;
//  } else if (commandS.find("|&")!= std::string::nops){
//      mIsPipeBackground= true;
//  } else if(commandS.find("|") != std::string::nops)
//  {
//      mIsPipeForeground= true;
//  }






void SmallShell::executeCommand(const char *cmd_line) {
    if(_trim(string(cmd_line)).compare("") == 0)
        return;
    Command* cmd= CreateCommand(cmd_line);
    char* args[COMMAND_ARGS_MAX_LENGTH+3];
    int num= _parseCommandLine(cmd_line,args);
    this->mpJobsList->RemoveFinishedJobs();
    cmd->execute(args,num);
    for (int i = 0; i <  num; ++i) {
        if(args[i]) (free(args[i]));
    }
    delete cmd;


    // TODO: Add your implementation here
    // for example:
    // Command* cmd = CreateCommand(cmd_line);
    // cmd->execute();
    // Please note that you must fork smash process for some commands (e.g., external commands....)
}