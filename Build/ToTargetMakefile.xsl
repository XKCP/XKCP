<?xml version='1.0' encoding="UTF-8"?>
<!--
Implementation by Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
-->
<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    version='1.0'>

<xsl:output method="text" indent="no" encoding="UTF-8"/>

<xsl:template name="filename">
    <xsl:param name="fullPath"/>
    <xsl:choose>
        <xsl:when test="contains($fullPath, '/')">
            <xsl:call-template name="filename">
                <xsl:with-param name="fullPath" select="substring-after($fullPath, '/')"/>
            </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
            <xsl:value-of select="$fullPath"/>
        </xsl:otherwise>
    </xsl:choose>
</xsl:template>

<xsl:template name="getFileNameWithoutExtension">
    <xsl:param name="fullPath"/>
    <xsl:choose>
        <xsl:when test="contains($fullPath, '/')">
            <xsl:call-template name="getFileNameWithoutExtension">
                <xsl:with-param name="fullPath" select="substring-after($fullPath, '/')"/>
            </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
            <xsl:value-of select="substring-before($fullPath, '.')"/>
        </xsl:otherwise>
    </xsl:choose>
</xsl:template>

<xsl:template name="getFilePrefix">
    <xsl:param name="fullPath"/>
    <xsl:param name="prefix"/>
    <xsl:choose>
        <xsl:when test="contains($fullPath, '/')">
            <xsl:call-template name="getFilePrefix">
                <xsl:with-param name="fullPath" select="substring-after($fullPath, '/')"/>
                <xsl:with-param name="prefix" select="concat($prefix, substring-before($fullPath, '/'), '/')"/>
            </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
            <xsl:value-of select="$prefix"/>
        </xsl:otherwise>
    </xsl:choose>
</xsl:template>

<xsl:template match="gcc">
    <xsl:text>CFLAGS := $(CFLAGS) </xsl:text>
    <xsl:value-of select="."/>
    <xsl:text>

</xsl:text>
</xsl:template>

<xsl:template match="define">
    <xsl:text>CFLAGS := $(CFLAGS) -D</xsl:text>
    <xsl:value-of select="."/>
    <xsl:if test="@as">="<xsl:value-of select="@as"/>"</xsl:if>
    <xsl:text>

</xsl:text>
</xsl:template>

<xsl:template match="h">
    <xsl:text>HEADERS := $(HEADERS) </xsl:text>
    <xsl:value-of select="."/>
    <xsl:text>
</xsl:text>
    <xsl:text>SOURCES := $(SOURCES) </xsl:text>
    <xsl:value-of select="."/>
    <xsl:text>
</xsl:text>
    <xsl:text>CFLAGS := $(CFLAGS) -I</xsl:text>
    <xsl:call-template name="getFilePrefix">
        <xsl:with-param name="fullPath" select="."/>
    </xsl:call-template>
    <xsl:text>

</xsl:text>
</xsl:template>

<xsl:template match="c">
    <xsl:text>SOURCES := $(SOURCES) </xsl:text>
    <xsl:value-of select="."/>
    <xsl:text>
</xsl:text>
    <xsl:variable name="object">
        <xsl:text>$(BINDIR)/</xsl:text>
        <xsl:call-template name="getFileNameWithoutExtension">
            <xsl:with-param name="fullPath" select="."/>
        </xsl:call-template>
        <xsl:text>.o</xsl:text>
    </xsl:variable>
    <xsl:value-of select="$object"/>
    <xsl:text>: </xsl:text>
    <xsl:value-of select="."/>
    <xsl:text> $(HEADERS)
&#9;$(CC) $(INCLUDES) $(CFLAGS) -c $&lt; -o $@
OBJECTS := $(OBJECTS) </xsl:text>
    <xsl:value-of select="$object"/>
    <xsl:text>

</xsl:text>
</xsl:template>

<xsl:template match="text()"/>

<xsl:template match="target">
    <xsl:variable name="final" select="concat('bin/', @name)"/>
    <xsl:variable name="pack" select="concat('bin/', translate(@name, '/', '_'), '.tar.gz')"/>

    <xsl:text>all: </xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text>
</xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text>: </xsl:text>
    <xsl:value-of select="$final"/>
    <xsl:text>
</xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text>.pack: </xsl:text>
    <xsl:value-of select="$pack"/>
    <xsl:text>

</xsl:text>
    <xsl:text>BINDIR = bin/.build/</xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text>
$(BINDIR):
&#9;mkdir -p $(BINDIR)

MAKE ?= gmake
CC ?= gcc
AR = ar

</xsl:text>
    <xsl:apply-templates select="gcc|define"/>
    <xsl:apply-templates select="h|hpp"/>
    <xsl:apply-templates select="c|cpp"/>

    <xsl:text>bin/</xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text>: $(BINDIR) $(OBJECTS)
&#9;mkdir -p $(dir $@)
</xsl:text>

    <xsl:choose>
        <xsl:when test="substring(@name, string-length(@name)-1, 2)='.a'">
            <xsl:text>&#9;mkdir -p $@.headers
&#9;cp -f $(HEADERS) $@.headers/
&#9;$(AR) rcsv $@ $(OBJECTS)
</xsl:text>
        </xsl:when>
        <xsl:otherwise>
            <xsl:text>&#9;$(CC) -o $@ $(OBJECTS) $(CFLAGS)
</xsl:text>
        </xsl:otherwise>
    </xsl:choose>

    <xsl:value-of select="$pack"/>
    <xsl:text>: $(SOURCES)
&#9;mkdir -p bin/.pack/</xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text>
&#9;rm -rf bin/.pack/</xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text>/*
&#9;cp $(SOURCES) bin/.pack/</xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text>/
&#9;cd bin/.pack/ ; tar -czf </xsl:text>
    <xsl:value-of select="concat('../../', $pack)"/>
    <xsl:text> </xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text>/*

</xsl:text>
</xsl:template>

</xsl:stylesheet>
