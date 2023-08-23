#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"

using namespace std;

void ctrlZHandler(int sig_num) {
    SmallShell& smash=SmallShell::getInstance();
    std::cout<<"smash: got ctrl-Z"<<std::endl;

    if(smash.mCurrentFg== nullptr){
//        perror("ztrl z error happened: mCurrentFg is nullptr, there is no process that is running in the foreground");
        return;
    }
//    std::cout<<"smash: got ctrl-Z"<<std::endl;

    if(kill(smash.mCurrentFg->mPid,SIGSTOP)==-1){
        perror("Smash error: kill failed");
        return;
    }
    auto it=smash.mpJobsList->GetJobByPid(smash.mCurrentFg->mPid);
    if (it != nullptr){
        it->mJobId=smash.mCurrentFg->mJobId;
        it->mJobStatus=gotCtrlZ;

        it->isBackground= false;
        it->command=smash.mCurrentFg->command;
        it->mEntryTime= time(nullptr);//check this: if we add a job in background, then get it
        //to foreground, and then we ctrl-z, it should return to the list as a stopped job
        //with that entry time? the original or a new one, because on the pdf it says
        //"Since job was inserted to the jobs list"
        std::cout<<"smash: process "<<smash.mCurrentFg->mPid<<" was stopped"<<std::endl;
        smash.mCurrentFg= nullptr;
        return;
    }
    for (JobsList::JobEntry& job: smash.mpJobsList->list) {
        if(job.mJobStatus==empty){
            job.mPid=smash.mCurrentFg->mPid;
            job.mJobStatus=gotCtrlZ;
            job.isBackground= false;
            string* t=new string(smash.mCurrentFg->command);
            job.command= *t;


//            string t =smash.mCurrentFg->command;
            job.mJobId=smash.mpJobsList->GetMaxId()+1;
            job.mEntryTime= time(nullptr);
            std::cout<<"smash: process "<<smash.mCurrentFg->mPid<<" was stopped"<<std::endl;
            smash.mCurrentFg= nullptr;
            return;
        }

    }

}

void ctrlCHandler(int sig_num) {
    SmallShell& smash=SmallShell::getInstance();
    std::cout<< "smash: got ctrl-C"<<std::endl;
    if(smash.mCurrentFg== nullptr){
        return;
    }
    if(kill(smash.mCurrentFg->mPid,SIGTERM) ==-1){
        perror("smash error: kill failed");
        return;
    }
//    bool found= false;
//    for (JobsList::JobEntry& job: smash.mpJobsList->list) {
//        if(smash.mCurrentFg->mPid==job.mPid) found= true;
//    }
    auto it=smash.mpJobsList->GetJobByPid(smash.mCurrentFg->mPid);
    if(it != nullptr){
        it->mJobStatus=finished;
//        smash.mCurrentFg= nullptr;
    }
//    if(it== nullptr){
//        for (JobsList::JobEntry& job: smash.mpJobsList->list) {
//            if(job.mJobStatus==empty){
//                job.mJobStatus=finished;
//                job.
//            }
//        }
//    } else{
//
//    }
//    if(found){
//        smash.mpJobsList->GetJobById(smash.mCurrentFg->mPid)->mJobStatus=finished;
//    }
//    if(!found){
//
//    }
    std::cout<<"smash: process "<<smash.mCurrentFg->mPid<<" was killed" << std::endl;
    smash.mCurrentFg->mJobStatus=finished;
    smash.mCurrentFg=nullptr;
    return;

}

void alarmHandler(int sig_num) {
    // TODO: Add your implementation
}

