#ifndef QTZ_SECURITY_VIRTUAL_MACHINE_DETECTOR_HPP
#define QTZ_SECURITY_VIRTUAL_MACHINE_DETECTOR_HPP

#include <QString>
#include <cstddef>

class VirtualMachineDetector
{
public:
    VirtualMachineDetector();
    ~VirtualMachineDetector() = default;
    VirtualMachineDetector(const VirtualMachineDetector&) = delete;
    bool isReal() const;
    bool isVirtual() const;
    void number_of_cores() const;
    bool checkCommand(const QString &command,
                      const QString &likelihood) const;
    void registry_check() const;
private:
    mutable int vm_score;
};

#endif // QTZ_SECURITY_VIRTUAL_MACHINE_DETECTOR_HPP
