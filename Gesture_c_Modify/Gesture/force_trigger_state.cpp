

#include "force_process.h"
#include "force_trigger_state.h"
#include "gesture_fastpress_norelease_state.h"
extern  TF_PARAMETER g_tTFParaMeter;
void force_trigger_state(void)
{
  if(g_tForceTriggerStatus == FORCE_NOTRIGGER)         //未触发过
  {
    if(g_ulTF_force >= g_tTFParaMeter.g_lMotorTrigger) //力度大于60
    {
      g_tForceTriggerStatus = FORCE_TRIGGERED;         //换成触发状态
    }
  }

  else if(g_tForceTriggerStatus == FORCE_TRIGGERED)   //已经触发
  {
    if(g_ulTF_force < g_tTFParaMeter.g_lMotorRelease) //力度小于30
    {
      g_tForceTriggerStatus = FORCE_NOTRIGGER;        //换成未触发状态
      //motorTriggered = 0;
      ClearFastPress();
    }
  }
}