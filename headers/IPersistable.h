class IPersistable {
public:
    virtual void save_to_db() const = 0;

    virtual void load_from_db(int id) = 0;

    virtual ~IPersistable() = default;
};
