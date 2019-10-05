#include "ring_buffer.h"
#define HISTORY_SIZE 4500

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
    const MeteoState& GetCurrentState() const
    {
      return m_data.back();
    }

    void PushNewState(MeteoState&& state)
    {
      m_data.push_back((MeteoState&&)state);
    }

    size_t size() const
    {
      return m_data.size();
    }

    const MeteoState& operator[](size_t idx) const
    {
      return m_data[idx];
    }
  private:
    ring_buffer<MeteoState, HISTORY_SIZE> m_data;
};
