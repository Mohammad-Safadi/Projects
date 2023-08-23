#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_
//#define _GNU_SOURCE
#include <memory>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
#include <map>
#include <cstring>
#include <iterator>
#include "fstream"
#include <sys/stat.h>
#include <fcntl.h>



#define COMMAND_ARGS_MAX_LENGTH (200)
#define ARGS_ARRAY_LENGTH (202)
#define COMMAND_MAX_ARGS (20)
#define JOBS_MAX_AMOUNT (100)


typedef enum { finished = 0, stopped = 1, background = 2, empty = 3, foreground=4, gotCtrlZ=5 }JobStatus;
class JobsList;

class Command {
public:
    std::string mNewCmd;
    bool IsCommandBuiltIn=false;
    bool mIsCommandBackground=false;
    Command(const char* cmdLine);
    virtual ~Command(){};
    virtual void execute(char** args=NULL,int numOfArgs=0) =0;
    bool IsBuiltInCommand(){
        return IsCommandBuiltIn;
    }
    bool IsBackgroundCommand(){
        return mIsCommandBackground;
    }
    //virtual void prepare();
    //virtual void cleanup();
    // TODO: Add your extra methods if needed
};

class BuiltInCommand : public Command {
public:
    BuiltInCommand(const char* cmd_line);
    virtual ~BuiltInCommand() {}
    void execute(char** args=NULL,int numOfArgs=0)override;

};

class ChPromptCommand: public BuiltInCommand{
public:
    ChPromptCommand(const char* cmd);
    virtual ~ChPromptCommand(){};
    void execute(char** args= nullptr, int num=0) override;


};

class ExternalCommand : public Command {
public:
    ExternalCommand(const char* cmd_line);
    virtual ~ExternalCommand() {}
    void execute(char** args=NULL , int num=0) override;
};

class PipeCommand : public Command {
    // TODO: Add your data members
public:
    bool mBackgroundPipe;
    PipeCommand(const char* cmd_line,bool background);
    virtual ~PipeCommand() {}
    void execute(char** args=NULL , int num=0) override;
};

class RedirectionCommand : public Command {
    // TODO: Add your data members
public:
    bool append;

    RedirectionCommand(const char* cmd_line,bool append);
    virtual ~RedirectionCommand() {}
    void execute(char** args=NULL , int num=0) override;
    //void prepare() override;
    //void cleanup() override;
};

class ChangeDirCommand : public BuiltInCommand {
public:
// TODO: Add your data members public:
    ChangeDirCommand(const char* cmd_line);
    virtual ~ChangeDirCommand() {}
    void execute(char** args=NULL , int num=0) override;
};

class GetCurrDirCommand : public BuiltInCommand {
public:
    GetCurrDirCommand(const char* cmd_line);
    virtual ~GetCurrDirCommand() {}
    void execute(char** args=NULL , int num=0) override;
};

class ShowPidCommand : public BuiltInCommand {
public:
    ShowPidCommand(const char* cmd_line);
    virtual ~ShowPidCommand() {}
    void execute(char** args=NULL, int num=0) override;
};

class JobsList;
class QuitCommand : public BuiltInCommand {
public:
// TODO: Add your data members public:
    QuitCommand(const char* cmd_line);
    virtual ~QuitCommand() {}
    void execute(char** args=NULL, int num=0) override;
};




class JobsList {
public:
    class JobEntry {
    public:
        JobStatus mJobStatus;
        bool isBackground;
        int mJobId;
        pid_t mPid;
        time_t mEntryTime;
        std::string command;

        JobEntry():mJobStatus(empty),isBackground(false),mJobId(0),mPid(0), mEntryTime(0){};
        ~JobEntry()=default;
    };

    std::vector<JobEntry> list;


    int GetMaxId();
    JobsList():list(JOBS_MAX_AMOUNT*2){};
    ~JobsList()=default;
    int GetNumOfJobs();
    void AddJob(pid_t pid);
    void PrintJobsListForQuit();
    void PrintJobsList();
    void KillAllJobs();
    void RemoveFinishedJobs();
    JobEntry* GetJobById(int id);
    bool IsThereAnyStopped();
    JobEntry* GetJobByPid(int Pid);
    bool IsEmpty();
    void RemoveJobById(int id);
    JobEntry* GetLastJob();
    JobEntry* GetLastStoppedJob();
    JobEntry* GetTimeoutCommand();


// public:
//  //JobsList() : array(JOBS_MAX_AMOUNT), nonBuiltInCommand(JOBS_MAX_AMOUNT), stopped(JOBS_MAX_AMOUNT){}
//  ~JobsList()=default;
//  void addJob(Command* cmd, bool isStopped = false){
//      vector<JobEntry>& = cmd
//  };
//  void printJobsList();
//  void killAllJobs();
//  void removeFinishedJobs();
//  JobEntry * getJobById(int jobId);
//  void removeJobById(int jobId);
//  JobEntry * getLastJob(int* lastJobId);
//  JobEntry *getLastStoppedJob(int *jobId);
    // TODO: Add extra methods or modify exisitng ones as needed
};

class JobsCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    JobsCommand(const char* cmd_line);
    virtual ~JobsCommand() {}
    void execute(char** args=NULL, int num=0) override;
};

class KillCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    KillCommand(const char* cmd_line);
    virtual ~KillCommand() {}
    void execute(char** args=NULL, int num=0) override;
};

class ForegroundCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    ForegroundCommand(const char* cmd_line);
    virtual ~ForegroundCommand() {}
    void execute(char** args=NULL, int num=0) override;
};

class BackgroundCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    BackgroundCommand(const char* cmd_line);
    virtual ~BackgroundCommand() {}
    void execute(char** args=NULL, int num=0) override;
};

class HeadCommand : public BuiltInCommand {
public:
    HeadCommand(const char* cmd_line);
    virtual ~HeadCommand() {}
    void execute(char** args=NULL , int num=0) override;
};


class SmallShell {
public:  // TODO: Add your data members

    JobsList::JobEntry* mCurrentFg;
    char* mPrevPwd;
    bool mUseCustomPrompt;
    JobsList* mpJobsList;
    JobsList newlist;
//    std::string mNewLine;
    pid_t mPid;
    char* mLastPrompt;//may be changed to char*


    bool IsJobsListEmpty(){
        return mpJobsList->IsEmpty();
    };


    SmallShell();
    Command *CreateCommand(const char* cmd_line);
    SmallShell(SmallShell const&)      = delete; // disable copy ctor
    void operator=(SmallShell const&)  = delete; // disable = operator
    static SmallShell& getInstance() // make SmallShell singleton
    {
        static SmallShell instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    ~SmallShell();
    void executeCommand(const char* cmd_line);
    // TODO: add extra methods as needed
};

#endif //SMASH_COMMAND_H_
