<?xml version='1.0' encoding="UTF-8"?>
<!--
The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
Michaël Peeters and Gilles Van Assche. For more information, feedback or
questions, please refer to our website: http://keccak.noekeon.org/

Implementation by Ronny Van Keer and the designers,
hereby denoted as "the implementer".

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
-->
<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    version='1.0'>

<xsl:output method="text" indent="no" encoding="UTF-8"/>

<xsl:template match="target">
    <xsl:variable name="targetfile" select="concat('bin/build/', @name, '.target')"/>
    <xsl:variable name="makefile" select="concat('bin/build/', @name, '.make')"/>
    <xsl:variable name="object" select="concat('bin/', @name)"/>
    <xsl:variable name="pack" select="concat(@name, '.pack')"/>

    <xsl:text>.PHONY: </xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text> </xsl:text>
    <xsl:value-of select="$object"/>
    <xsl:text> </xsl:text>
    <xsl:value-of select="$pack"/>
    <xsl:text>
</xsl:text>

    <xsl:value-of select="@name"/>
    <xsl:text>: </xsl:text>
    <xsl:value-of select="$object"/>
    <xsl:text>
</xsl:text>

    <xsl:value-of select="$object"/>
    <xsl:text>: </xsl:text>
    <xsl:value-of select="$makefile"/>
    <xsl:text>
&#9;make -f </xsl:text>
    <xsl:value-of select="$makefile"/>
    <xsl:text> </xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text>
</xsl:text>

    <xsl:value-of select="$pack"/>
    <xsl:text>: </xsl:text>
    <xsl:value-of select="$makefile"/>
    <xsl:text>
&#9;make -f </xsl:text>
    <xsl:value-of select="$makefile"/>
    <xsl:text> </xsl:text>
    <xsl:value-of select="$pack"/>
    <xsl:text>
</xsl:text>

    <xsl:value-of select="$makefile"/>
    <xsl:text>: </xsl:text>
    <xsl:value-of select="$targetfile"/>
    <xsl:text> Build/ToTargetMakefile.xsl
&#9;xsltproc -o $@ Build/ToTargetMakefile.xsl $&lt;
</xsl:text>

    <xsl:value-of select="$targetfile"/>
    <xsl:text>: Build/ToOneTarget.xsl Makefile.build
&#9;xsltproc -o $@ -param nameTarget "'</xsl:text>
    <xsl:value-of select="@name"/>
    <xsl:text>'" Build/ToOneTarget.xsl Makefile.build

</xsl:text>
</xsl:template>

<xsl:template match="build">
    <xsl:apply-templates select="target"/>
</xsl:template>

</xsl:stylesheet>
