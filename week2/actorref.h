#ifndef ACTORREF_H
#define ACTORREF_H


template <typename T>
class ActorRef{
  private:
    T *m_object;

  public:
    ActorRef();
    ActorRef(T *);
    ActorRef(const class ActorRef<T> &);
    class ActorRef<T> & operator=(T *);
    class ActorRef<T> & operator=(const class ActorRef<T> &);
    T * operator->() const;
    operator bool() const;
    operator class IPlayer *() const;
    T * Get() const;
    bool operator<(const class ActorRef<T> &) const;
};

#endif