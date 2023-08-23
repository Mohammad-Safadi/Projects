#include "CarManager.h"
void *Init()
{
    CarManager *DS;
    try
    {
        DS = new CarManager();
    }
    catch (const std::bad_alloc &)
    {
        DS = nullptr;
    }

    return (void *)DS;
}
StatusType AddCarType(void *DS, int typeID, int numOfModels)
{
    if (DS==NULL)
    {
        return INVALID_INPUT;
    }
    if (typeID <= 0)
    {
        return INVALID_INPUT;
    };
    if (numOfModels <= 0)
    {
        return INVALID_INPUT;
    }
        // Failure if type already exists
        // Success


    return (((CarManager *)DS)->ADDCartype(typeID, numOfModels));
}

StatusType RemoveCarType(void *DS, int typeID)
{
    if (DS==NULL)
    {
        return INVALID_INPUT;
    }
    if (typeID<= 0)
    {
        return INVALID_INPUT;
    }
    return (((CarManager *)DS)->RemoveCarType(typeID));
}


StatusType SellCar(void *DS, int typeID, int modelID)
{
    if (DS==NULL)
    {
        return INVALID_INPUT;
    }
    if (typeID <= 0)
    {
        return INVALID_INPUT;
    }
    if (modelID < 0)
    {
        return INVALID_INPUT;
    }
    return(((CarManager *)DS)->SellCar(typeID,modelID));
}

StatusType MakeComplaint(void *DS, int typeID, int modelID, int t){
    if (DS==NULL)
    {
        return INVALID_INPUT;
    }
    if (typeID <= 0)
    {
        return INVALID_INPUT;
    }
    if (modelID < 0)
    {
        return INVALID_INPUT;
    }
    return(((CarManager *)DS)->MakeComplaint(typeID,modelID,t));
}

StatusType GetBestSellerModelByType(void *DS, int typeID, int * modelID){
    if (DS==NULL)
    {
        return INVALID_INPUT;
    }
    if(typeID<0)
    {
        return INVALID_INPUT;
    }
    return(((CarManager *)DS)->GetBestSellerModelByType(typeID,modelID));
}

StatusType GetWorstModels(void *DS, int numOfModels, int *types, int *models)
{
    if (DS==NULL||types== NULL ||models==NULL)
    {
        return INVALID_INPUT;
    }
    if (numOfModels < 0)
    {
        return INVALID_INPUT;
    }
        return(((CarManager *)DS)->GetWorstModels(numOfModels, types,models));
}

void Quit(void **DS)
{
    delete (CarManager *)(*DS);
    *DS = NULL;
}