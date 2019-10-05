template<class T, size_t Size>
class ring_buffer_iterator;

template <class T, size_t Size>
class ring_buffer
{
  using Iterator = ring_buffer_iterator<T, Size>;
  public:
    void push_back(T&& value)
    {
      m_data[m_end] = value;
      m_end = (m_end + 1) % Size;
      if (m_start == m_end)
      {
        m_start = (m_start + 1) % Size;
      }
    }

    T pop_back()
    {
      m_end = m_end == 0 ? Size - 1 : m_end - 1;
      return m_data[m_end];
    }

    T pop_front()
    {
      m_start = (m_start + 1) % Size;
      return m_data[m_start];
    }

    const T& back() const
    {
      const size_t i = m_end == 0 ? Size - 1 : m_end - 1;
      return m_data[i];
    }

    T& back()
    {
      const size_t i = m_end == 0 ? Size - 1 : m_end - 1;
      return m_data[i];
    }

    size_t size() const
    {
      return (m_end - m_start + Size) % Size;
    }

    const T& operator[](size_t idx) const
    {
      return m_data[(m_start + idx) % Size];
    }

    T& operator[](size_t idx)
    {
      return m_data[(m_start + idx) % Size];
    }

    Iterator begin()
    {
      return Iterator(this, 0);
    }

    Iterator end()
    {
      return Iterator(this, size());
    }

  private:
    size_t m_start = 0;
    size_t m_end = 0;
    T m_data[Size] = {};
};

template<class T, size_t Size>
class ring_buffer_iterator
{
  using Buffer = ring_buffer<T, Size>;
  public:
    ring_buffer_iterator(Buffer* buffer, size_t idx)
    : m_buffer(buffer)
    , m_idx(idx)
    {
    }

    bool operator== (const ring_buffer_iterator& other) const
    {
      return m_buffer == other.m_buffer && m_idx == other.m_idx;
    }

    ring_buffer_iterator& operator++()
    {
      m_idx = (m_idx + 1 % Size);
      return *this;
    }

    T& operator*()
    {
      return (*m_buffer)[m_idx];
    }
    
    const T& operator*() const
    {
      return (*m_buffer)[m_idx];
    }
  private:
    Buffer* m_buffer;
    size_t m_idx;
};
