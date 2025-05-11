#ifndef IDENTITY_H
#define IDENTITY_H

#include <QString>



class Identity
{
public:
    enum job{novelist,explorer,entomologist,journalist};
    Identity();
    job role;
    QString imagePath;
    QString KaTongPath;

};

#endif // IDENTITY_H
