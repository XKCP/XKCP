<?xml version='1.0' encoding="UTF-8"?>
<!--
Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
denoted as "the implementer".

For more information, feedback or questions, please refer to our websites:
http://keccak.noekeon.org/
http://keyak.noekeon.org/
http://ketje.noekeon.org/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
-->
<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    version='1.0'>

<xsl:param name="nameTarget"/>

<xsl:template name="buildList">
    <xsl:param name="queued"/>
    <xsl:param name="explored" select="'|'"/>
    <xsl:variable name="first" select="substring-before(concat($queued, ' '), ' ')"/>
    <xsl:variable name="inheritedFromFirst" select="//*[@name=$first]/@inherits"/>
    <xsl:variable name="restOfQueue" select="substring-after($queued, ' ')"/>
    <xsl:choose>
        <xsl:when test="$first=''">
            <xsl:value-of select="$explored"/>
        </xsl:when>
        <xsl:when test="contains($explored, concat('|', $first, '|'))">
            <xsl:call-template name="buildList">
                <xsl:with-param name="queued" select="normalize-space($restOfQueue)"/>
                <xsl:with-param name="explored" select="$explored"/>
            </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
            <xsl:call-template name="buildList">
                <xsl:with-param name="queued" select="normalize-space(concat($restOfQueue, ' ', $inheritedFromFirst))"/>
                <xsl:with-param name="explored" select="concat($explored, $first, '|')"/>
            </xsl:call-template>
        </xsl:otherwise>
    </xsl:choose>

</xsl:template>

<xsl:template match="build">
    <xsl:variable name="listFragments">
        <xsl:call-template name="buildList">
            <xsl:with-param name="queued" select="$nameTarget"/>
        </xsl:call-template>
    </xsl:variable>
    <target name="{$nameTarget}">
        <xsl:apply-templates select="//*[contains($listFragments, concat('|',@name,'|'))]"/>
    </target>
</xsl:template>

<xsl:template match="target|fragment">
    <xsl:apply-templates select="*"/>
</xsl:template>

<xsl:template match="*">
    <xsl:copy-of select="."/>
</xsl:template>

</xsl:stylesheet>
