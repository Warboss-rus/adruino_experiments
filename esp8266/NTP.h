class NTPTime
{
  public:
    void init();    
    unsigned long getTimeStamp() const;
    
  private:
    unsigned long m_time = 0;
    unsigned long m_timeRecievedAt = 0;
};
