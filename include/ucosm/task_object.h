/*
 * Copyright (C) 2020 Thomas AUBERT <aubert.thms@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name ``Thomas AUBERT'' nor the name of any other
 *    contributor may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 
 * uCosmDev IS PROVIDED BY Thomas AUBERT ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL Thomas AUBERT OR ANY OTHER CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <limits>

#include "itask.h"

#include "void_m.h"

#include "ucosm_sys_data.h"
uint8_t UcosmSysData::sCnt = 0;

template<uint32_t task_count, typename module_M = void_M>
class TaskObject: public ITask {

    using task_index_t = uint8_t;

    static_assert(task_count <= std::numeric_limits<task_index_t>::max(), "Task count too high");

    static_assert(task_count > 0, "Task count must be at least 1");

public:

    TaskObject() :
            mTaskCount(0), mIdleTask(nullptr) {
    }

    // add a child task to schedule
    // returns true if success, false otherwise
    bool addTask(ITask *inTask);

    // removes a child task
    // returns true is success, false otherwise
    bool removeTask(ITask *inTask);

    // returns the associated module
    // returns nullptr if task doesn't exist
    module_M* getTask(ITask *inTask);

    // set the free or static function to execute
    // when no child task is executed
    void setIdleTask(void (*inIdleTask)());

    // to call periodically
    // manually or through another instance
    // returns true if a task has been executed,
    // false otherwise
    bool schedule() final;

private:

    bool getTaskIndex(ITask *inScheduler, task_index_t &ioIndex);

    ITask *mTasks[task_count];

    module_M mTaskTraits[task_count];

    task_index_t mTaskCount;

    void (*mIdleTask)();

};

template<uint32_t task_count, typename module_M>
bool TaskObject<task_count, module_M>::addTask(ITask *inTask) {
    if (mTaskCount == task_count) {
        return false;
    }
    mTasks[mTaskCount] = inTask;
    mTaskTraits[mTaskCount].init();
    mTaskCount++;
    return true;
}

template<uint32_t task_count, typename module_M>
module_M* TaskObject<task_count, module_M>::getTask(ITask *inTask) {
    task_index_t i;
    if (getTaskIndex(inTask, i)) {
        return &mTaskTraits[i];
    }
    return nullptr;
}

template<uint32_t task_count, typename module_M>
bool TaskObject<task_count, module_M>::removeTask(ITask *inTask) {
    task_index_t i;
    if (getTaskIndex(inTask, i)) {

        if (!mTaskTraits[i].isDelReady()) {
            return false;
        }

        mTaskTraits[i].makePreDel();

        // shift tasks for contiguous array
        while (i < mTaskCount - 1) {
            mTasks[i] = mTasks[i + 1];
            mTaskTraits[i] = mTaskTraits[i + 1];
            i++;
        }
        mTaskCount--;
    }
    return true;
}

template<uint32_t task_count, typename module_M>
bool TaskObject<task_count, module_M>::schedule() {

    bool hasWork = false;

    task_index_t i = 0;

    UcosmSysData::sCnt++;

    while (i < mTaskCount) {

        if (mTasks[i] && mTaskTraits[i].isExeReady()) {
            mTaskTraits[i].makePreExe();
            hasWork |= mTasks[i]->schedule();
            mTaskTraits[i].makePostExe();
        }
        i++;
    }

    if (!hasWork && mIdleTask) {
        // idle task if exists
        mIdleTask();
    }

    return hasWork;
}

template<uint32_t task_count, typename module_M>
void TaskObject<task_count, module_M>::setIdleTask(void (*inIdleTask)()) {
    mIdleTask = inIdleTask;
}

template<uint32_t task_count, typename module_M>
bool TaskObject<task_count, module_M>::
getTaskIndex(ITask *inScheduler, task_index_t &ioIndex) {
    if (!mTaskCount) {
        return false;
    }
    ioIndex = 0;
    do {
        if (mTasks[ioIndex] == inScheduler) {
            return true;
        }
    } while (++ioIndex < mTaskCount);
    return false;
}
