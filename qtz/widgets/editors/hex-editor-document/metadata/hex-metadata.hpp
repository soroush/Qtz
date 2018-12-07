#ifndef QTZ_WIDGETS_HEXMETADATA_HPP
#define QTZ_WIDGETS_HEXMETADATA_HPP

#include <QObject>
#include <QList>
#include <QHash>
#include <QMap>
#include "hex-metadata-item.hpp"
#include "../../../qtz-widgets.hpp"

typedef QList<HexMetadataItem*> MetadataList;
typedef QHash<integer_t, MetadataList> MetadataMap;

class QTZ_WIDGETS_SHARED_EXPORT HexMetadata : public QObject {
    Q_OBJECT

private:
    typedef QMap<integer_t, integer_t> RangeMap;
    typedef QPair<RangeMap::const_iterator, RangeMap::const_iterator> RangeIterators;

public:
    explicit HexMetadata(QObject* parent = 0);
    void insert(HexMetadataItem* metaitem);
    void beginMetadata();
    void endMetadata();
    void clearHighlighting();
    void clearComments();
    MetadataList fromOffset(integer_t offset) const;
    QString commentString(integer_t offset) const;

private:
    const HexMetadataItem* comment(integer_t offset) const;

signals:
    void metadataChanged();

private:
    RangeMap _ranges;
    RangeIterators _lastrange;
    MetadataMap _metadata;
    bool _bulkmetadata;
};

#endif // QTZ_WIDGETS_HEXMETADATA_HPP
