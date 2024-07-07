#ifndef AIZONE_H
#define AIZONE_H

#include <string>
#include <set>

class AIZoneListener {
  private:
    class AIZone *m_zone;

  public:
    AIZoneListener();
    virtual ~AIZoneListener();
    void SetAIZone(const std::string &);
    virtual void OnAIZoneActivated();
    virtual void OnAIZoneDeactivated();
    bool IsAIZoneActive();
};

class AIZone {
  private:
    std::string m_name;
    size_t m_playerCount;
    class std::set<AIZoneListener*, std::less<AIZoneListener*>, std::allocator<AIZoneListener*> > m_listeners;

  public:
    AIZone(const std::string &);
    const std::string & GetName() const;
    bool IsActive() const;
    void OnPlayerEntered();
    void OnPlayerLeft();
    void AddListener(class AIZoneListener *);
    void RemoveListener(class AIZoneListener *);
};

#endif