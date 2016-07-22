#include "microOS.h"

system_info_t			MicroOS::_system_info = NOINFO;
uint8_t					MicroOS::_system_request = NOREQUEST;
float					MicroOS::_gpin_float[8] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
int32_t					MicroOS::_gpin_int[4] = {0,0,0,0};
uint16_t 				MicroOS::_gpin_changed = 0;
float					MicroOS::_gpout_float[8] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
int32_t					MicroOS::_gpout_int[4] = {0,0,0,0};
uint16_t 				MicroOS::_gpout_changed = 0;
uint8_t 				MicroOS::_thread_count = 0;
uint8_t 				MicroOS::_next_thread = 0;
uint8_t 				MicroOS::_scheduled_thread = 0;
Thread**				MicroOS::_threads;
HALBase* 				MicroOS::_hal;
CommunicatorInterface* 	MicroOS::_communicator;
uint8_t					MicroOS::_slowhook_splitcounter = 0;

int MicroOS::microOSSlowLoop(void)
{
	switch(_slowhook_splitcounter){
		case 0:
			_hal->onboardLedToggle();
			_communicator->sendHeartbeat();
			break;
		case 1:
			handleSystemRequest();
			break;
	}
	
	if(++_slowhook_splitcounter > 1)
		_slowhook_splitcounter = 0;
	
	return 0;
}

int MicroOS::microOSFastLoop(void)
{
	_communicator->sendGPIO();
	_communicator->receive();
	return 0;
}


//TODO: handle steppers properly
/*int MicroOS::stepperControl(void)
{
	for(uint8_t k=0;k<HAL_INTERFACE_STEPPER_ENABLE;k++){
	#ifdef STEPPERCONTROL_FIXEDTIME 
		_hal->getStepper(k)->update(STEPPERCONTROL_FIXEDTIME);
	#else
		_hal->getStepper(k)->update();
	#endif
	}
	return 0;
}*/

uint8_t MicroOS::init(HALBase* hal, CommunicatorInterface* communicator, uint8_t config)
{
	

	//TODO: Test spi begin/enable
	if((config & MICROOS_I2C_ENABLE) != 0)
		Wire.begin(); 
	/*if((config & MICROOS_SPI_ENABLE) != 0)
		SPI.begin();*/

	_hal = hal;
	_hal->init();
	_communicator = communicator;
	_communicator->init();
	
	// Add the different standard threads
	if((config & MICROOS_ONB_DISABLE) == 0)
		addThread(BELOWNORMAL, MICROOS_SLOW_THREAD_PERIOD, &MicroOS::microOSSlowLoop, false, MICROOS_SLOW_THREAD_ID);
	if((config & MICROOS_COM_DISABLE) == 0)
		addThread(NORMAL, MICROOS_FAST_THREAD_PERIOD, &MicroOS::microOSFastLoop, false, MICROOS_FAST_THREAD_ID);
	//addThread(STEPPERCONTROL_PRIORITY, STEPPERCONTROL_TIME, &MicroOS::stepperControl, false, STEPPERCONTROL_ID);
	
	return 0;
}

uint8_t MicroOS::init(CommunicatorInterface* communicator, uint8_t config)
{
	MicroOS::init(new HALBase(),communicator,config);
}

void MicroOS::start(const start_t mode)
{
	switch(mode){
		case SEQUENTIAL:
			for(uint8_t k=0;k<_thread_count;k++){
				_threads[k]->startSequential();
			}
			break;
		case REGULAR:{
			uint32_t interval, minimum_period, scheduling_time;
			for(uint8_t k=0;k<_thread_count;k++){
				if(_threads[k]->getPeriod() < minimum_period)
					minimum_period = _threads[k]->getPeriod();
			}
			interval = minimum_period/_thread_count;
			scheduling_time = micros();
			for(uint8_t k=0;k<_thread_count;k++){
				_threads[k]->startRegular(scheduling_time);
				scheduling_time += interval;
			}
		}
	}
}

void MicroOS::run(const system_run_t mode)
{
	switch(mode){
		case RESCHEDULED:{
			if(_threads[_scheduled_thread]->action()){
				for(uint8_t k=0;k<_thread_count;k++){
					if(_threads[_scheduled_thread]->getWakeupTime() > _threads[k]->getWakeupTime()){
						_scheduled_thread = k;
					}
				}
				
			}
			break;}
	
		case PRIORITIZED:
			for(uint8_t k=0;k<_thread_count;k++){
				if(_threads[k]->action()){
					break;
				}
			}
			break;
	}
}

HALBase* MicroOS::hal()
{
	return _hal;
}

int MicroOS::findThread(uint8_t ID)
{
	for(uint8_t k=0;k<_thread_count;k++){
		if(_threads[k]->getID()==ID){
			return k;
		}
	}
	
	return -1;
}


uint8_t MicroOS::addThread(priority_t priority, uint32_t period, int (*Fcn)(), bool start, uint8_t ID)
{	
    Thread *thread = new Thread(priority, period, Fcn);
    thread->setID(ID);	
    uint8_t newID = addThread(thread);
    
    if(start)
        thread->start();
        
    return newID;
}

uint8_t MicroOS::addThread(Thread *thread)
{
	uint8_t k = 0;
	bool IDtaken = false;
	
	// Check if the requested ID is still available
	if((thread->getID() == 0) || (findThread(thread->getID()) >= 0)){
		IDtaken = true;
	}
	
	// Search the list for an ID which has not yet been assigned
	while(IDtaken){
		thread->setID(thread->getID() + 1); 			//new search ID
		IDtaken = (findThread(thread->getID()) >= 0);	//test if it already exists
	}
	
	// Create new list of threads: one longer than before
	Thread **newThreads = new Thread*[_thread_count+1];
	
	// Add threads of higher priority first
	if(_thread_count > 0){
	    while((k < _thread_count) && (thread->getPriority() <= _threads[k]->getPriority())){
		    newThreads[k] = _threads[k];
		    k++;
	    }
	}
	
	// Add the new thread after all higher priority threads have taken their place
	newThreads[k] = thread;
	k++; _thread_count++;
	
	// Add all threads of lower priority
	while(k<_thread_count){
		newThreads[k] = _threads[k-1];
		k++;
	}
	
	// Delete the array with pointers to the threads and assign new array
	delete[] _threads;	
	_threads = newThreads;
	
	// Debug: list current thread ids
	/*Serial.println("Current thread IDs..");
	for(k=0;k<_thread_count;k++){
	    Serial.println(_threads[k]->getID());
	}*/
	
	// Return the ID of the newly added thread
	return thread->getID();
}

Thread *MicroOS::thread(uint8_t ID)
{
	uint8_t k = findThread(ID);

	if(k>0)
		return _threads[k];
	else
		return NULL;
}

void MicroOS::sendSystemRequest(uint8_t system_request)
{
	_system_request = system_request;
}

void MicroOS::handleSystemRequest()
{
	switch(_system_request){
		case NOREQUEST: break;
		case THREADINFO:{
			for(uint8_t k=0;k<_thread_count;k++){
				_communicator->sendThreadInfo(_threads[k]->getID(), _threads[k]->getName(), _threads[k]->getPriority(), 
										   _threads[k]->getDuration(), _threads[k]->getLatency(), 
										   _threads[k]->getTotalDuration(), _threads[k]->getTotalLatency(), _threads[k]->getNumberOfExecutions());
			}
		
			break;}
		case NEXTTHREADINFO:{
			_communicator->sendThreadInfo(_threads[_next_thread]->getID(), _threads[_next_thread]->getName(), _threads[_next_thread]->getPriority(), 
										  _threads[_next_thread]->getDuration(), _threads[_next_thread]->getLatency(), 
										  _threads[_next_thread]->getTotalDuration(), _threads[_next_thread]->getTotalLatency(), _threads[_next_thread]->getNumberOfExecutions());
			if(++_next_thread >= _thread_count)
				_next_thread = 0;
			break;}
		case HWINFO:{
			//do nothing for now
			break;}
	}
	
	_system_request = NOREQUEST;
}


float MicroOS::getGPinFloat(uint8_t index)
{
	if(index<8){
		return _gpin_float[index];
	} else {
		return 0;
	}
}

int32_t MicroOS::getGPinInt(uint8_t index)
{
	if(index<4){
		return _gpin_int[index];
	} else {
		return 0;
	}
}

bool MicroOS::getGPinFloatChanged(uint8_t index)
{
	if(index<8){
		return ((_gpin_changed & (1<<index)) != 0);
	} else {
		return false;
	}
}

bool MicroOS::getGPinIntChanged(uint8_t index)
{
	if(index<4){
		return ((_gpin_changed & (1<<(index+8))) != 0);
	} else {
		return false;
	}
}

void MicroOS::setGPinFloat(uint8_t index, float value)
{
	if((index<8) && (_gpin_float[index]!=value)){
		_gpin_float[index] = value;
		_gpin_changed |= (1<<index);
	}
}

void MicroOS::setGPinInt(uint8_t index, int32_t value)
{
	if(index<4 && (_gpin_int[index]!=value)){
		_gpin_int[index] = value;
		_gpin_changed |= (1<<(index+4));
	}
}

float* MicroOS::getGPoutFloat(void)
{
	return _gpout_float;
}

int32_t* MicroOS::getGPoutInt(void)
{
	return _gpout_int;
}

void MicroOS::setGPoutFloat(uint8_t index, float value)
{
	if(index<8){
		_gpout_float[index] = value;
		_gpout_changed |= (1<<index);
	}
}

void MicroOS::setGPoutInt(uint8_t index, int32_t value)
{
	if(index<4){
		_gpout_int[index] = value;
		_gpout_changed |= (1<<(index+8));
	}
}


