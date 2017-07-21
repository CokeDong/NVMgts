#include "Schedular.h"
#include <string>
#include <fstream>


//Queue:
int initMyQueue(myQueue *q, int nCount)
{
	q->m_nCount = nCount;
	//q->m_pData = new TYPE[nCount];//ע�����ﲻ��С���ţ�С�����ǵ��ù��캯��
	q->m_pData = (TYPE*)p_malloc(sizeof(TYPE)*nCount);
	p_bind(6,q->m_pData,sizeof(TYPE));
	q->m_nHead = q->m_nTail = 0;
}

bool isEmpty(myQueue *q)
{
	return q->m_nHead == q->m_nTail;
}
bool isFull(myQueue *q)//�������ѵ�
{
	return (q->m_nTail+1)%q->m_nCount == q->m_nHead; //β�� ��1 ���ܳ���ȡ ���� ����� ͷ�� ��ȣ��������������ҪԤ����һ���ռ����ж��Ƿ�������
}

void push(myQueue *q,const TYPE& t)
{
	if (isFull(q))
	{
		return;
	}
	q->m_pData[q->m_nTail++] = t;
	q->m_nTail %= q->m_nCount;  //��� β�� �����������ֱ���ܵ� ��ͷ


}
bool pop(myQueue *q)
{
	if (isEmpty(q))
	{
		return false;
	}
	q->m_nHead++;
	// t = m_pData[];
	q->m_nTail %= q->m_nCount;
	return true;

}

TYPE* front(myQueue *q)
{
	TYPE* t;
	if (isEmpty(q))
	{
		return NULL;
	}
	t = &q->m_pData[q->m_nHead];
	q->m_nTail %= q->m_nCount;
	return t;
}



//Schedular::Schedular()
//{
//    //ctor
//    this->lastCompletedJob = -1;
//}
//
//Schedular::~Schedular()
//{
//    //dtor
//}

int runSchedular(Schedular *sche, Grid *gridIndex, Trajectory *DB){
    sche->gridIndex = gridIndex;
    sche->DB = DB;
    if(sche->lastCompletedJob == -1)// have not been inited
    {
        //have not been inited, build queue in nvm
        initSchedular(sche, gridIndex, DB);
        std::ifstream in("queryList.txt",std::ios::in);
        char queryStr[1024];
        while(!in.eof())
        {
            in.getline(queryStr,1024);
            printf("%s",queryStr);
            if(queryStr[0] == '\0')
                break;
            // generate job
            Job newJob;
            newJob.jobID = sche->jobIDMax++;
            char *tokenPtr = strtok(queryStr, ",");
            newJob.queryMBR.xmin = atof(tokenPtr);
            newJob.queryMBR.xmax = atof(strtok(NULL, ","));
            newJob.queryMBR.ymin = atof(strtok(NULL, ","));
            newJob.queryMBR.ymax = atof(strtok(NULL, ","));
            newJob.queryTime = time(NULL);
            newJob.queryTimeClock = clock();
            //sche->jobsBuffQueue->push(newJob);
            push(sche->jobsBuffQueue,newJob);
        }
        in.close();
        sche->lastCompletedJob = 0;
    }
    else
        //�����ͷ��Ȼ������ʽ�������Ĵ���
    {
        //schedular has been initialed, recover from nvm
        //remember to add the base addr
        int sz;
        char *baseAddr = (char*)p_get_base();
        sche->jobsBuffQueue = (myQueue*)p_get_bind_node(5,&sz);
        sche->jobsBuffQueue->m_pData = (TYPE*)p_get_bind_node(6,&sz);
        sche->fp = fopen("Performance.txt","a+");
        Job *pJob = front(sche->jobsBuffQueue);
        if(pJob==NULL){
            cout << "all jobs have been handled, if you want to reload, enter R and then press enter." << endl;
            string press;
            cin >> press;
            if(press == "R"){
                std::ifstream in("queryList.txt",std::ios::in);
                char queryStr[1024];
                while(!in.eof())
                {
                    in.getline(queryStr,1024);
                    printf("%s",queryStr);
                    if(queryStr[0] == '\0')
                        break;
                    // generate job
                    Job newJob;
                    newJob.jobID = sche->jobIDMax++;
                    char *tokenPtr = strtok(queryStr, ",");
                    newJob.queryMBR.xmin = atof(tokenPtr);
                    newJob.queryMBR.xmax = atof(strtok(NULL, ","));
                    newJob.queryMBR.ymin = atof(strtok(NULL, ","));
                    newJob.queryMBR.ymax = atof(strtok(NULL, ","));
                    newJob.queryTime = time(NULL);
                    newJob.queryTimeClock = clock();
                    //sche->jobsBuffQueue->push(newJob);
                    push(sche->jobsBuffQueue,newJob);
                }
                in.close();
                sche->lastCompletedJob = 0;
                pJob = front(sche->jobsBuffQueue);
            }
            else
                return 0;
        }
        if((pJob->commited == false) && (pJob->completed == true))
        {
            //sche->writeResult();
            writeResult(sche);
            // ֻ�������ύ�������
            pJob->completeTime = time(NULL);
            pJob->completeTimeClock = clock();
            // ���������5s��˵��������������ͨ��ʱ���Ϳ���
            if(difftime(pJob->completeTime,pJob->queryTime)>5)
            {
                fprintf(sche->fp,"Query %d use time %f s\n",pJob->jobID,difftime(pJob->completeTime,pJob->queryTime));
                printf("Query %d use time %f s\n",pJob->jobID,difftime(pJob->completeTime,pJob->queryTime));
            }
            // ������С��5s��˵�����ܿ�����clock��ȷ��ʱ
            else
            {
                fprintf(sche->fp,"Query %d use time %f s\n",pJob->jobID,double(pJob->completeTimeClock-pJob->queryTimeClock)/1000);
                printf("Query %d use time %f s\n",pJob->jobID,double(pJob->completeTimeClock-pJob->queryTimeClock)/1000);
            }
            fflush(sche->fp);
            pJob->commited = true;
            pop(sche->jobsBuffQueue);
        }
        else if((pJob->commited == false) && (pJob->completed == false))
        {
            executeQueryInSchedular(sche);
            writeResult(sche);
            pJob->completed = true;
            pJob->completeTime = time(NULL);
            pJob->completeTimeClock = clock();
            // ���������5s��˵��������������ͨ��ʱ���Ϳ���
            if(difftime(pJob->completeTime,pJob->queryTime)>5)
            {
                fprintf(sche->fp,"Query %d use time %f s\n",pJob->jobID,difftime(pJob->completeTime,pJob->queryTime));
                printf("Query %d use time %f s\n",pJob->jobID,difftime(pJob->completeTime,pJob->queryTime));
            }
            // ������С��5s��˵�����ܿ�����clock��ȷ��ʱ
            else
            {
                fprintf(sche->fp,"Query %d use time %f s\n",pJob->jobID,double(pJob->completeTimeClock-pJob->queryTimeClock)/1000);
                printf("Query %d use time %f s\n",pJob->jobID,double(pJob->completeTimeClock-pJob->queryTimeClock)/1000);
            }
            fflush(sche->fp);
            pJob->commited = true;
            pop(sche->jobsBuffQueue);
        }
        else
        {
            pop(sche->jobsBuffQueue);
        }
    }
    while(!isEmpty(sche->jobsBuffQueue))
    {
        //execute the query
        Job *pJob = front(sche->jobsBuffQueue);
        executeQueryInSchedular(sche);
        pJob->completed = true;
        writeResult(sche);
        pJob->completeTime = time(NULL);
        pJob->completeTimeClock = clock();
        // ���������5s��˵��������������ͨ��ʱ���Ϳ���
        if(difftime(pJob->completeTime,pJob->queryTime)>5)
        {
            fprintf(sche->fp,"Query %d use time %f s\n",pJob->jobID,difftime(pJob->completeTime,pJob->queryTime));
            printf("Query %d use time %f s\n",pJob->jobID,difftime(pJob->completeTime,pJob->queryTime));
        }
        // ������С��5s��˵�����ܿ�����clock��ȷ��ʱ
        else
        {
            fprintf(sche->fp,"Query %d use time %f s\n",pJob->jobID,double(pJob->completeTimeClock-pJob->queryTimeClock)/1000);
            printf("Query %d use time %f s\n",pJob->jobID,double(pJob->completeTimeClock-pJob->queryTimeClock)/1000);
        }
        fflush(sche->fp);
        pJob->commited = true;
        pop(sche->jobsBuffQueue);
    }
    fclose(sche->fp);
}

//int Schedular::run(Grid *gridIndex, Trajectory *DB)
//{
//    // ���ϵͳ��ʼ��������ʼ�����У�Ȼ������ʽ�������Ĵ���
//
//}


int initSchedular(Schedular *sche, Grid *gridIndex, Trajectory *DB){
    //sche->jobsBuffQueue = (myQueue*)malloc(sizeof(myQueue));
    sche->jobsBuffQueue = (myQueue*)p_malloc(sizeof(myQueue));
    p_bind(5,sche->jobsBuffQueue,sizeof(myQueue));
    initMyQueue(sche->jobsBuffQueue,MAXJOBSNUM);
    sche->gridIndex = gridIndex;
    sche->DB = DB;
    sche->fp = fopen("Performance.txt","a+");
    sche->jobIDMax = 0;
    sche->lastCompletedJob = -1;

}

//int Schedular::init(Grid *gridIndex, Trajectory *DB)
//{
//    this->jobsBuffQueue = new myQueue(MAXJOBSNUM);
//    this->gridIndex = gridIndex;
//    this->DB = DB;
//    this->fp = fopen("Performance.txt","a+");
//    this->jobIDMax = 0;
//    this->lastCompletedJob = -1;
//}

//ִ�в�ѯ�������д��job������

int executeQueryInSchedular(Schedular *sche){
    Job *pJob = front(sche->jobsBuffQueue);
    //this->gridIndex->rangeQuery(pJob->queryMBR,pJob->resultData,&pJob->resultNum); //���������д����DRAM���malloc������
    rangeQuery(sche->gridIndex,pJob->queryMBR,pJob->resultData,&pJob->resultNum);
}



//int Schedular::executeQuery()
//{
//    Job *pJob = this->jobsBuffQueue->front();
//    //this->gridIndex->rangeQuery(pJob->queryMBR,pJob->resultData,&pJob->resultNum); //���������д����DRAM���malloc������
//    rangeQuery(this->gridIndex,pJob->queryMBR,pJob->resultData,&pJob->resultNum);
//}

//int Schedular::loadJobs()
//{
//
//}

int writeResult(Schedular *sche){
    Job *pJob = front(sche->jobsBuffQueue);
    int resultNum = pJob->resultNum;
    CPURangeQueryResult* pStart = pJob->resultData;
    CPURangeQueryResult *pLast=NULL;
    FILE *fp = fopen("RangeQueryResult.txt","a+");
    fprintf(fp,"Query ID:%d, Result Num:%d\n",pJob->jobID,pJob->resultNum);
    for(int i=0;i<=resultNum-1;i++)
    {
        if(pStart!=NULL)
        {
            fprintf(fp,"%f,%f,%d\n",pStart->x,pStart->y,pStart->traid);
            pLast = pStart;
            pStart = pStart->next;
            free(pLast);
        }
    }
    fclose(fp);
}

//
//-----------------------------
// how to permit atom?
//-----------------------------
//
bool destroySchedular(Schedular *sche){
    myQueue* tempQueue = sche->jobsBuffQueue;
    p_free(tempQueue->m_pData);
    p_free(sche->jobsBuffQueue);
    p_free(sche);
    (*stateData) = 3;
}

//
//int Schedular::writeResult()
//{
//    Job *pJob = this->jobsBuffQueue->front();
//    int resultNum = pJob->resultNum;
//    CPURangeQueryResult* pStart = pJob->resultData;
//    CPURangeQueryResult *pLast=NULL;
//    FILE *fp = fopen("RangeQueryResult.txt","a+");
//    fprintf(fp,"Query ID:%d, Result Num:%d\n",pJob->jobID,pJob->resultNum);
//    for(int i=0;i<=resultNum-1;i++)
//    {
//        if(pStart!=NULL)
//        {
//            fprintf(fp,"%f,%f,%d\n",pStart->x,pStart->y,pStart->traid);
//            pLast = pStart;
//            pStart = pStart->next;
//            free(pLast);
//        }
//    }
//    fclose(fp);
//}
