#define HISTORY_SIZE 100

struct MeteoState
{
  float temperature;
  float humidity;
  unsigned int pressure;
  unsigned long timestamp;
};

class MeteoStorage
{
  public:
    MeteoState GetCurrentState() const
    {
      return m_currentState;
    }

    void PushNewState(const MeteoState& state)
    {
      m_history[m_historyEnd] = state;
      m_historyEnd = (m_historyEnd + 1) % HISTORY_SIZE;
      if (m_historyStart == m_historyEnd)
      {
        m_historyStart = (m_historyStart + 1) % HISTORY_SIZE;
      }
      m_currentState = state;
    }

    size_t size() const
    {
      return (m_historyEnd - m_historyStart + HISTORY_SIZE) % HISTORY_SIZE;
    }

    const MeteoState& operator[](size_t idx) const
    {
      return m_history[(m_historyStart + idx) % HISTORY_SIZE];
    }
  private:
    MeteoState m_currentState = {0.0f, 0, 0.0f};
    MeteoState m_history[HISTORY_SIZE] = {};
    unsigned int m_historyStart = 0;
    unsigned int m_historyEnd = 0;
};
