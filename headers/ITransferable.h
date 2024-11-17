class ITransferable {
public:
    virtual void transfer_money(const std::string &from_wallet, const std::string &to_wallet, double amount) = 0;
    virtual ~ITransferable() = default;
};
