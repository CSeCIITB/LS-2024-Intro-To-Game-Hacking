#ifndef NPC_H
#define NPC_H

#include "actor.h"

enum NPCStateTransitionType {EndConversationTransition, ContinueConversationTransition, ShopTransition};

struct NPCStateTransition {
    std::string text;
    enum NPCStateTransitionType type;
    std::string nextState;
};

struct NPCState {
    std::string text;
    class std::vector<NPCStateTransition, std::allocator<NPCStateTransition> > transitions;
};
class NPC : public Actor {
  private:
    class std::map<std::string, NPCState, std::less<std::string>, std::allocator<std::pair<std::string const, NPCState> > > m_states;

  public:
    NPC(const std::string &);
    virtual bool IsNPC();
    void AddState(const std::string &, const std::string &);
    void AddStateTransition(const std::string &, const std::string &, const std::string &);
    void AddStateTransitionToEnd(const std::string &, const std::string &);
    void AddStateTransitionToShop(const std::string &, const std::string &);
    std::string GetTextForState(const std::string &);
    class std::vector<NPCStateTransition, std::allocator<NPCStateTransition> > GetTransitionsForState(const std::string &);
    virtual std::string GetInitialState(class IPlayer *);
    virtual void OnTransitionTaken(class IPlayer *, const std::string &, const std::string &);
    virtual bool CanUse(class IPlayer *);
    virtual void PerformUse(class IPlayer *);
    virtual int32_t GetBuyPriceForItem(class IItem *);
    virtual int32_t GetSellPriceForItem(class IItem *);
};

#endif